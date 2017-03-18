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

#define RABBITMQ_CONSUMER_NUM 1
#define CONSUMER_NAME "consumer_for_sender"

extern std::string g_rabbitmq_server_ip;
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

template<int N>
class Rabbitmq_consumer_pool
{
public:
	void receive_respond_msg();
    void send_msg(const std::string& target_exchange, const std::string& target_mod, const std::string& msg, msg_callback_fun cb, void* arg);
    static Rabbitmq_consumer_pool<N>* instance(const std::string& rabbitmq_server_ip);
	~Rabbitmq_consumer_pool();
private:
	Rabbitmq_consumer_pool() {}
	Rabbitmq_consumer_pool(const std::string & rabbitmq_server_ip);
	std::unordered_map<int, std::shared_ptr<Message_completion>> m_completions_map;
	std::mutex m_completions_map_lck;
	std::string m_rabbitmq_server_ip;
    std::string m_reply_exchange;
	AmqpClient::Channel::ptr_t m_send_pchannel;
	int m_channel_num;
	void new_and_add_completion(int msg_id, msg_callback_fun cb, void* arg);
	std::shared_ptr<Message_completion> find_and_remove_completion(int msg_id);
};

template<int N>
Rabbitmq_consumer_pool<N>::Rabbitmq_consumer_pool(const std::string & rabbitmq_server_ip)
	:m_rabbitmq_server_ip(rabbitmq_server_ip),
     m_reply_exchange("10.0.0.2")
{
    m_send_pchannel = AmqpClient::Channel::Create(m_rabbitmq_server_ip);

    for(int i = 0; i < m_channel_num; i++)
        {
            std::thread(&Rabbitmq_consumer_pool::receive_respond_msg, this).detach();
        }
}

template<int N>
Rabbitmq_consumer_pool<N>* Rabbitmq_consumer_pool<N>::instance(const std::string& rabbitmq_server_ip)
{
	static std::unordered_map<std::string, std::shared_ptr<Rabbitmq_consumer_pool<N>>> consumer_map;
	static std::mutex consumer_map_lck;
	Rabbitmq_consumer_pool* rp;
	std::lock_guard<std::mutex> __lock_coumer_map(consumer_map_lck);
	auto itor = consumer_map.find(rabbitmq_server_ip);
	if (consumer_map.end() != itor)
	{
		rp = itor->second.get();
	}
	else
	{
		rp = new Rabbitmq_consumer_pool(rabbitmq_server_ip);
		consumer_map.insert({ rabbitmq_server_ip, std::shared_ptr<Rabbitmq_consumer_pool<N>>(rp) });
	}
	return rp;
}



template<int N>
std::shared_ptr<Message_completion> Rabbitmq_consumer_pool<N>::find_and_remove_completion(int msg_id)
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

template<int N>
void Rabbitmq_consumer_pool<N>::new_and_add_completion(int msg_id, msg_callback_fun cb, void* arg)
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

template<int N>
void Rabbitmq_consumer_pool<N>::receive_respond_msg()
{
    AmqpClient::Channel::ptr_t pchannel = AmqpClient::Channel::Create(m_rabbitmq_server_ip);
    const std::string reply_queue_name = pchannel->DeclareQueue("");
    pchannel->BindQueue( reply_queue_name, m_reply_exchange, "reply" );

    // 在一个queue上面订阅消息 这个queue上的所有消息都会被传送到
    std::string consumer_tag = pchannel->BasicConsume(reply_queue_name);

	for (;;)
    {
		AmqpClient::Envelope::ptr_t envelope = 	pchannel->BasicConsumeMessage(consumer_tag);
        if(nullptr == envelope.get()){
             break;
        }
		std::string message_id = envelope->Message()->MessageId();
		int msg_id = atoi(message_id.c_str());
		std::shared_ptr<std::string> pMsg(new std::string(envelope->Message()->Body()));
		std::shared_ptr<Message_completion> pCompletion = find_and_remove_completion(msg_id);
        if(nullptr == pCompletion){
            //
        }
		pCompletion->pfunc(pMsg, pCompletion->arg);
    }
}

/*
 * sonsumer_pool的send_msg
 */

template<int N>
void Rabbitmq_consumer_pool<N>::send_msg(const std::string& target_exchange,
										 const std::string& target_mod,
										 const std::string& msg,
										 msg_callback_fun cb,
										 void* arg)
{
	AmqpClient::BasicMessage::ptr_t message
		= AmqpClient::BasicMessage::Create(msg);
	int msg_id = generate_msg_id();
	const std::string message_id = std::to_string(msg_id);
	message->MessageId(message_id);
	new_and_add_completion(msg_id, cb, arg);
	m_send_pchannel->BasicPublish(target_exchange, target_mod, message);
}

/*
 * 建立一个consumer_pool的实例 调用 consumer_pool 的 send_msg
 */
static void send_msg(const std::string& target_exchange,
					 const std::string& target_mod,
					 const std::string& msg,
					 msg_callback_fun cb,
					 void* arg)
{
	Rabbitmq_consumer_pool<RABBITMQ_CONSUMER_NUM>::instance(g_rabbitmq_server_ip)->send_msg(target_exchange, target_mod, msg, cb, arg);
}

template<int N>
Rabbitmq_consumer_pool<N>::~Rabbitmq_consumer_pool()
{
}



class Rabbitmq_sender
{

public:
	Rabbitmq_sender(const std::string& target_addr, int port);
	virtual ~Rabbitmq_sender();
    int send_msg_no_reply(const std::string& target_mod, const std::string& msg);
    int send_msg_sync(const std::string& target_mod, const std::string& msg_s, std::shared_ptr<std::string>& msg_reply);
    int send_msg_async(const std::string& target_mod, const std::string& msg_s, msg_callback_fun cb, void* arg) ;
private:
    std::string m_send_exchange;
    int m_target_port;
};

Rabbitmq_sender::Rabbitmq_sender(const std::string& target_addr,int port)
    :m_send_exchange(target_addr),m_target_port(port)
{
	return;
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

int Rabbitmq_sender::send_msg_no_reply(const std::string& target_mod, const std::string& msg)
{
   	send_msg(m_send_exchange,target_mod, msg, nullptr, nullptr);
    return 0;
}

int Rabbitmq_sender::send_msg_sync(const std::string& target_mod, const std::string& msg_s, std::shared_ptr<std::string>& msg_reply)
{
	Sync_send_arg arg;
	arg.complete = false;
	send_msg(m_send_exchange, target_mod, msg_s, sync_send_completion, &arg);
	std::unique_lock < std::mutex > _locker(arg.lck);
	while (!arg.complete)
	{
		arg.m_cond_var.wait(_locker);
	}
	msg_reply = arg.p_reply_msg;
    return 0;
}

int Rabbitmq_sender::send_msg_async(const std::string& target_mod, const std::string& msg_s, msg_callback_fun cb, void* arg)
{
	send_msg(m_send_exchange, target_mod, msg_s, cb, arg);
    return 0;
}
