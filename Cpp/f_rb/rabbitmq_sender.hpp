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

typedef AmqpClient::Channel::ptr_t rabbitmq_channel_ptr;

//callback function of rabbitmq send msessage invoke
typedef void(*msg_callback_fun)(const std::shared_ptr<std::string> pMsg, void *arg);

/*
 * callback paramenter of rabbitmq send invoke
 */
typedef struct Msg_completion_str
{
	msg_callback_fun pfunc;
	void *arg;
} Message_completion;

class Rabbitmq_channel_pool
{
public:
	~Rabbitmq_channel_pool()
	{};

	rabbitmq_channel_ptr get_rb_channel();

	void init_pool(const std::string &rabbitmq_server_addr)
	{
		m_rabbitmq_server_addr = rabbitmq_server_addr;
	}

private:
	Rabbitmq_channel_pool()
	{};
	std::unordered_map<std::thread::id, rabbitmq_channel_ptr> m_channel_map;
	std::mutex m_channel_map_lck;
	std::string m_rabbitmq_server_addr;

	DECLARE_SIGNLETON_CLASS(Rabbitmq_channel_pool)
};

inline rabbitmq_channel_ptr Rabbitmq_channel_pool::get_rb_channel()
{
	std::thread::id key = std::this_thread::get_id();
	std::lock_guard<std::mutex> __lck_channel_map(m_channel_map_lck);
	auto itor = m_channel_map.find(key);
	if ( m_channel_map.end() != itor )
	{
		return itor->second;
	} else
	{
		try
		{
			assert(m_rabbitmq_server_addr.length() > 0);
			rabbitmq_channel_ptr p = AmqpClient::Channel::Create(m_rabbitmq_server_addr);
			m_channel_map.insert({key, p});
			return p;
		}
		catch ( std::exception &e )
		{
			//throw error_exception("connect to rabbitmq fail","j");
		}
	}
}

static inline rabbitmq_channel_ptr get_rb_channel()
{
	return Singleton<Rabbitmq_channel_pool>::instance()->get_rb_channel();
}

static inline uint64_t generate_msg_id()
{
	static std::atomic<uint64_t> id(0);
	return id++;
}

class Rabbitmq_sender_proxy
{
public:
	void send_msg(const std::string &routing_key,
				  const std::string &target_mod,
				  const std::string &msg,
				  msg_callback_fun cb,
				  void *arg);

	void init_sender_proxy(const std::string &rabbitmq_server_addr,
						   const std::string &respond_addr,
						   int32_t respond_port,
						   const std::string &rabbitmq_sender_exchang,
						   const std::string &rabbitmq_receiver_exchang)
	{
		m_rabbitmq_sender_exchang = rabbitmq_sender_exchang;
		m_rabbitmq_receiver_exchang = rabbitmq_receiver_exchang;
		m_rabbitmq_server_addr = rabbitmq_server_addr;
		m_reply_routing_key = respond_addr + ":" + std::to_string(respond_port);
		m_pchannel = AmqpClient::Channel::Create(m_rabbitmq_server_addr);
		m_queue_name = m_pchannel->DeclareQueue("");
		m_pchannel->BindQueue(m_queue_name, m_rabbitmq_receiver_exchang, m_reply_routing_key);
		std::thread(&Rabbitmq_sender_proxy::receive_respond_msg, this).detach();
	}

	~Rabbitmq_sender_proxy();

private:
	Rabbitmq_sender_proxy();

	void receive_respond_msg();

	void new_and_add_completion(int msg_id, msg_callback_fun cb, void *arg);

	std::shared_ptr<Message_completion> find_and_remove_completion(int msg_id);

	std::unordered_map<int, std::shared_ptr<Message_completion>> m_completions_map;
	std::mutex m_completions_map_lck;
	rabbitmq_channel_ptr m_pchannel;
	std::string m_queue_name;
	std::string m_reply_routing_key;
	std::string m_rabbitmq_server_addr;
	std::string m_rabbitmq_sender_exchang;
	std::string m_rabbitmq_receiver_exchang;

	DECLARE_SIGNLETON_CLASS(Rabbitmq_sender_proxy)
};

std::shared_ptr<Message_completion> Rabbitmq_sender_proxy::find_and_remove_completion(int msg_id)
{
	std::shared_ptr<Message_completion> pc(nullptr);
	{
		std::lock_guard<std::mutex> __locker(m_completions_map_lck);
		auto it = m_completions_map.find(msg_id);
		if ( it != m_completions_map.end())
		{
			pc = it->second;
			m_completions_map.erase(it);
		}
	}
	return pc;
}

void Rabbitmq_sender_proxy::new_and_add_completion(int msg_id, msg_callback_fun cb, void *arg)
{
	std::shared_ptr<Message_completion> pc(new Message_completion);
	if ( 0 == pc.get())
	{
		//throw error_exception("alloc Message_completion fail", ALLOCATE_FAIL);
	}
	pc->pfunc = cb;
	pc->arg = arg;
	std::lock_guard<std::mutex> __locker(m_completions_map_lck);
	m_completions_map.insert({msg_id, pc});
	return;
}

//声明 queue
//绑定 queue exchange reply_routing_key

Rabbitmq_sender_proxy::Rabbitmq_sender_proxy()
{
}

Rabbitmq_sender_proxy::~Rabbitmq_sender_proxy()
{
}

void Rabbitmq_sender_proxy::receive_respond_msg()
{
	std::string consumer_tag = m_pchannel->BasicConsume(m_queue_name);
	for ( ;; )
	{
		AmqpClient::Envelope::ptr_t envelope = m_pchannel->BasicConsumeMessage(consumer_tag);
		if ( envelope.get() == nullptr )
		{
			continue;
		}
		std::string message_id = envelope->Message()->MessageId();
		uint64_t msg_id = strtoul(message_id.c_str(), 0, 0);
		std::shared_ptr<std::string> pMsg(new std::string(envelope->Message()->Body()));
		std::shared_ptr<Message_completion> pCompletion = find_and_remove_completion(msg_id);
		pCompletion->pfunc(pMsg, pCompletion->arg);
	}
}

// 得到一个channel 封装msg  调用publish
void Rabbitmq_sender_proxy::send_msg(const std::string &send_routing_key, const std::string &target_mod,
									 const std::string &msg, msg_callback_fun cb, void *arg)
{
	AmqpClient::BasicMessage::ptr_t message
		= AmqpClient::BasicMessage::Create(msg);
	uint64_t msg_id = generate_msg_id();
	const std::string message_id = std::to_string(msg_id);
	message->ReplyTo(m_reply_routing_key); // 把ReplyTo 设置为返回的 reply_routing_key 而不是queue
	message->MessageId(message_id);
	//message->AppId(target_mod);
	new_and_add_completion(msg_id, cb, arg);
	rabbitmq_channel_ptr s_channel = get_rb_channel();
	s_channel->BasicPublish(m_rabbitmq_sender_exchang, target_mod + send_routing_key , message); //  target_mod + send_routing_key
}

static void send_msg(const std::string &send_routing_key,
					 const std::string &target_mod,
					 const std::string &msg,
					 msg_callback_fun cb,
					 void *arg)
{
	Singleton<Rabbitmq_sender_proxy>::instance()->send_msg(send_routing_key, target_mod, msg, cb, arg);
}

class Rabbitmq_sender
{

public:
	Rabbitmq_sender(const std::string &target_ip, int target_port);

	virtual ~Rabbitmq_sender();

	int send_msg_no_reply(const std::string &target_mod, const std::string &msg);

	int send_msg_sync(const std::string &target_mod, const std::string &msg_s, std::shared_ptr<std::string> &msg_reply);

	int send_msg_async(const std::string &target_mod, const std::string &msg_s, msg_callback_fun cb, void *arg);

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

Rabbitmq_sender::Rabbitmq_sender(const std::string &target_ip, int target_port)
	: m_target_ip(target_ip),
	  m_target_port(target_port)
{
	m_send_routing_key = m_target_ip + ":" + std::to_string(m_target_port);
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

void static sync_send_completion(std::shared_ptr<std::string> pMsg, void *arg)
{
	Sync_send_arg *sc = static_cast<Sync_send_arg *>(arg);
	std::unique_lock<std::mutex> _locker(sc->lck);
	sc->complete = true;
	sc->p_reply_msg = pMsg;
	sc->m_cond_var.notify_one();
}

int Rabbitmq_sender::send_msg_no_reply(const std::string &target_mod,
									   const std::string &msg)
{
	send_msg(m_send_routing_key, target_mod, msg, nullptr, nullptr);
	return 0;
}

int Rabbitmq_sender::send_msg_sync(const std::string &target_mod,
								   const std::string &msg_s,
								   std::shared_ptr<std::string> &msg_reply)
{
	Sync_send_arg arg;
	arg.complete = false;
	send_msg(m_send_routing_key, target_mod, msg_s, sync_send_completion, &arg);
	std::unique_lock<std::mutex> _locker(arg.lck);
	while ( !arg.complete )
	{
		arg.m_cond_var.wait(_locker);
	}
	msg_reply = arg.p_reply_msg;
	return 0;
}

int Rabbitmq_sender::send_msg_async(const std::string &target_mod,
									const std::string &msg_s, msg_callback_fun cb,
									void *arg)
{
	send_msg(m_send_routing_key, target_mod, msg_s, cb, arg);
	return 0;
}

static inline void rabbitmq_sender_init(const std::string &rabbitmq_server_addr,
										const std::string &respond_addr,
										int32_t respond_port,
										const std::string &rabbitmq_sender_exchange,
										const std::string &rabbitmq_receiver_exchange)
{
	Singleton<Rabbitmq_channel_pool>::instance()->init_pool(rabbitmq_server_addr);
	Singleton<Rabbitmq_sender_proxy>::instance()->init_sender_proxy(rabbitmq_server_addr,
																	respond_addr,
																	respond_port,
																	rabbitmq_sender_exchange,
																	rabbitmq_receiver_exchange);
}
