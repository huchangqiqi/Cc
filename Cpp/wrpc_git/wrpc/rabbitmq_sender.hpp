#include "singleton.h"
#include <atomic>
#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <unordered_map>

#define SEND_EXCHANGE "ex1"
#define RECEIVE_EXCHANGE "ex2"
#define G_RABBITMQ_SERVER_IP "10.9.5.31"

typedef AmqpClient::Channel::ptr_t c_ptr;
class Rabbitmq_channel_pool {
public:
	Rabbitmq_channel_pool(){};
	~Rabbitmq_channel_pool(){};
	c_ptr get_rb_channel();

private:
	std::unordered_map<std::thread::id, c_ptr> m_channel_map;
	std::mutex m_channel_map_lck;
	DECLARE_SIGNLETON_CLASS(Rabbitmq_channel_pool)
};

inline c_ptr Rabbitmq_channel_pool::get_rb_channel()
{
	std::thread::id key = std::this_thread::get_id();
	std::lock_guard<std::mutex> __lck_channel_map(m_channel_map_lck);
	auto itor = m_channel_map.find(key);
	if(m_channel_map.end() != itor)
	{
			return itor->second;
	}
	else
	{
		// read config file to get rabbitmq_server_addr
		try
		{
			c_ptr p = AmqpClient::Channel::Create(G_RABBITMQ_SERVER_IP);
			m_channel_map.insert({key,p});
			return p;
		}
		catch (std::exception& e)
		{
				//throw error_exception("connect to rabbitmq fail","j");
		}
	}
}

static inline c_ptr get_rb_channel()
{
	return Singleton<Rabbitmq_channel_pool>::instance()->get_rb_channel();
}


//std::string g_rabbitmq_server_addr;
typedef void(*msg_callback_fun)(const std::shared_ptr<std::string> pMsg, void* arg);
typedef struct Msg_completion_str
{
	msg_callback_fun pfunc;
	void* arg;
}Message_completion;

int generate_msg_id()
{
    static std::atomic_uint_fast32_t id(0) ;
    return id++;
}

class Rabbitmq_sender_proxy
{
public:
	void receive_respond_msg();
    void send_msg(const std::string& routing_key,const std::string& target_mod, const std::string& msg, msg_callback_fun cb, void* arg);
    static Rabbitmq_sender_proxy* instance(const std::string& reply_routing_key);
	~Rabbitmq_sender_proxy();
private:
	Rabbitmq_sender_proxy() {}
	Rabbitmq_sender_proxy(const std::string & module_name);
	std::unordered_map<int, std::shared_ptr<Message_completion>> m_completions_map;
	std::mutex m_completions_map_lck;
	void new_and_add_completion(int msg_id, msg_callback_fun cb, void* arg);
	std::shared_ptr<Message_completion> find_and_remove_completion(int msg_id);
    c_ptr m_pchannel;
    std::string m_queue_name ;
	std::string m_reply_routing_key;
};

std::shared_ptr<Message_completion> Rabbitmq_sender_proxy::find_and_remove_completion(int msg_id)
{
	std::shared_ptr<Message_completion> pc(nullptr);
	{
		std::lock_guard<std::mutex> __locker(m_completions_map_lck);
		auto it = m_completions_map.find(msg_id);
		if (it != m_completions_map.end())
		{
			pc = it->second;
			m_completions_map.erase(it);
		}
	}
	return pc;
}

void Rabbitmq_sender_proxy::new_and_add_completion(int msg_id, msg_callback_fun cb, void* arg)
{
	std::shared_ptr<Message_completion> pc(new Message_completion);
	if (0 == pc.get())
	{
		//throw error_exception("alloc Message_completion fail", ALLOCATE_FAIL);
	}
	pc->pfunc = cb;
	pc->arg = arg;
	std::lock_guard<std::mutex> __locker(m_completions_map_lck);
	m_completions_map.insert({ msg_id, pc });
	return;
}

//声明 queue
//绑定 queue exchange reply_routing_key

Rabbitmq_sender_proxy::Rabbitmq_sender_proxy(const std::string& reply_routing_key)
	:m_reply_routing_key(reply_routing_key)
{
    m_pchannel =  AmqpClient::Channel::Create(G_RABBITMQ_SERVER_IP);
    m_queue_name = m_pchannel->DeclareQueue("");
    m_pchannel->BindQueue( m_queue_name, RECEIVE_EXCHANGE, reply_routing_key);
 	std::thread(&Rabbitmq_sender_proxy::receive_respond_msg, this).detach();
}

// reply_routing_key ip+port
Rabbitmq_sender_proxy* Rabbitmq_sender_proxy::instance(const std::string& reply_routing_key)
{
	static std::unordered_map<std::string, std::shared_ptr<Rabbitmq_sender_proxy>> consumer_map;
	static std::mutex consumer_map_lck;
	Rabbitmq_sender_proxy* rb_consumer;
	std::lock_guard<std::mutex> __lock_coumer_map(consumer_map_lck);
	auto itor = consumer_map.find(reply_routing_key);
	if (consumer_map.end() != itor)
    {
		rb_consumer = itor->second.get();
    }
	else
    {
		rb_consumer = new Rabbitmq_sender_proxy(reply_routing_key);
		consumer_map.insert({reply_routing_key , std::shared_ptr<Rabbitmq_sender_proxy>(rb_consumer) });
    }
	return rb_consumer;
}

Rabbitmq_sender_proxy::~Rabbitmq_sender_proxy()
{
}

void Rabbitmq_sender_proxy::receive_respond_msg()
{
	std::string consumer_tag = m_pchannel->BasicConsume(m_queue_name);

	for (;;)
	{
        AmqpClient::Envelope::ptr_t envelope;
        m_pchannel->BasicConsumeMessage(consumer_tag, envelope, 100);
        if (envelope.get() == nullptr) {
            break;
        }
		std::string message_id = envelope->Message()->MessageId();
		int msg_id = atoi(message_id.c_str());
		std::shared_ptr<std::string> pMsg(new std::string(envelope->Message()->Body()));
		std::shared_ptr<Message_completion> pCompletion = find_and_remove_completion(msg_id);
		pCompletion->pfunc(pMsg, pCompletion->arg);
	}
}

// 得到一个channel 封装msg  调用publish
void Rabbitmq_sender_proxy::send_msg(const std::string& send_routing_key,const std::string& target_mod, const std::string& msg, msg_callback_fun cb, void* arg)
{
	AmqpClient::BasicMessage::ptr_t message
		= AmqpClient::BasicMessage::Create(msg);
	int msg_id = generate_msg_id();
	const std::string message_id = std::to_string(msg_id);
	message->ReplyTo(m_reply_routing_key); // 把ReplyTo 设置为返回的 reply_routing_key 而不是queue
	message->MessageId(message_id);
	message->AppId(target_mod);
	new_and_add_completion(msg_id, cb, arg);
    AmqpClient::Channel::ptr_t s_channel = get_rb_channel();
	s_channel->BasicPublish(SEND_EXCHANGE, send_routing_key, message);
}

static void send_msg(const std::string& routing_key,
					 const std::string& target_mod,
					 const std::string& msg,
					 msg_callback_fun cb,
					 void* arg)

{
	Rabbitmq_sender_proxy::instance("10.0.0.2:101")->send_msg(routing_key, target_mod, msg, cb, arg);
}

class Rabbitmq_sender
{

public:
	Rabbitmq_sender(const std::string& target_ip,int target_port);
	virtual ~Rabbitmq_sender();
    int send_msg_no_reply(const std::string& target_mod, const std::string& msg);
    int send_msg_sync(const std::string& target_mod, const std::string& msg_s, std::shared_ptr<std::string>& msg_reply);
    int send_msg_async(const std::string& target_mod, const std::string& msg_s, msg_callback_fun cb, void* arg) ;
	int init_rabbitmq_sender();
private:
	std::string m_target_ip;
	int m_target_port;
	std::string m_send_routing_key;
};

/*****************************************************************************
 Prototype    : Rabbitmq_sender.Rabbitmq_sender
 Description  : rabbitmq_sender 构造函数
 Input        : const std::string& target_addr
                int port
 Output       : None
 Return Value :

  History        :
  1.Date         : 2017/1/13
    Author       : xiaohua.du@ww-it.cn
    Modification : Created function

*****************************************************************************/

Rabbitmq_sender::Rabbitmq_sender(const std::string& target_ip, int target_port)
	:m_target_ip(target_ip),
	 m_target_port(target_port)
{
	m_send_routing_key = m_target_ip + ":" +std::to_string(m_target_port);
}

Rabbitmq_sender::~Rabbitmq_sender()
{
}

struct Sync_send_arg
{
	std::condition_variable m_cond_var;
	bool complete;
	std::shared_ptr<std::string> p_reply_msg;
	std::mutex lck;
};
void static sync_send_completion(std::shared_ptr<std::string> pMsg, void* arg)
{
	Sync_send_arg* sc= static_cast<Sync_send_arg*>(arg);
	std::unique_lock < std::mutex > _locker(sc->lck);
	sc->complete = true;
	sc->p_reply_msg = pMsg;
	sc->m_cond_var.notify_one();
}

int Rabbitmq_sender::send_msg_no_reply(const std::string& target_mod,
									   const std::string& msg)
{
   	send_msg(m_send_routing_key,target_mod, msg, nullptr, nullptr);
    return 0;
}

int Rabbitmq_sender::send_msg_sync(const std::string& target_mod,
								   const std::string& msg_s,
								   std::shared_ptr<std::string>& msg_reply)
{
	Sync_send_arg arg;
	arg.complete = false;
	send_msg(m_send_routing_key,target_mod, msg_s, sync_send_completion, &arg);
	std::unique_lock < std::mutex > _locker(arg.lck);
	while (!arg.complete)
	{
		arg.m_cond_var.wait(_locker);
	}
	msg_reply = arg.p_reply_msg;
    return 0;
}

int Rabbitmq_sender::send_msg_async(const std::string& target_mod,
                                    const std::string& msg_s, msg_callback_fun cb,
                                    void* arg)
{
	send_msg(m_send_routing_key,target_mod, msg_s, cb, arg);
    return 0;
}
