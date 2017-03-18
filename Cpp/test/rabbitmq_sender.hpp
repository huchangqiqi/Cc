#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#define RABBITMQ_CONSUMER_NUM 2
#define CONSUMER_NAME "consumer_for_sender"

extern std::string g_rabbitmq_server_ip;
typedef void (*msg_callback_fun)(const std::shared_ptr<std::string> pMsg,
                                 void* arg);
typedef struct Msg_completion_str {
    msg_callback_fun pfunc;
    void* arg;
} Message_completion;

int generate_msg_id() {
    static std::atomic_uint_fast32_t id(0);
    return id++;
}

template <int N>
class Rabbitmq_consumer_pool {
   public:
    void receive_respond_msg(AmqpClient::Channel::ptr_t& pchannel,
                             std::string& consumer);
    void send_msg(const std::string& target_exchange,
                  const std::string& target_mod, const std::string& msg,
                  msg_callback_fun cb, void* arg);
    static Rabbitmq_consumer_pool<N>* instance(
        const std::string& rabbitmq_server_ip);
    ~Rabbitmq_consumer_pool();

   private:
    Rabbitmq_consumer_pool() {}
    // throw fatal_exception("don't have default constructor", ALLOCATE_FAIL); }
    Rabbitmq_consumer_pool(const std::string& rabbitmq_server_ip);
    AmqpClient::Channel::ptr_t m_send_pchannel;
    std::unordered_map<int, std::shared_ptr<Message_completion>>
        m_completions_map;
    std::mutex m_completions_map_lck;
    std::vector<std::string> m_queue_name_vec;
    // std::vector<std::string> m_consumer_vec;
    std::string m_rabbitmq_server_ip;
    // std::vector< AmqpClient::Channel::ptr_t> m_pchannel_vec; //channel_vec
    int m_channel_num;  // channel_num
    void new_and_add_completion(int msg_id, msg_callback_fun cb, void* arg);
    std::shared_ptr<Message_completion> find_and_remove_completion(int msg_id);
};

template <int N>
Rabbitmq_consumer_pool<N>* Rabbitmq_consumer_pool<N>::instance(
    const std::string& rabbitmq_server_ip) {
    static std::unordered_map<std::string,
                              std::shared_ptr<Rabbitmq_consumer_pool<N>>>
        consumer_map;
    static std::mutex consumer_map_lck;
    Rabbitmq_consumer_pool* rp;
    std::lock_guard<std::mutex> __lock_coumer_map(consumer_map_lck);
    auto itor = consumer_map.find(rabbitmq_server_ip);
    if (consumer_map.end() != itor) {
        rp = itor->second.get();
    } else {
        rp = new Rabbitmq_consumer_pool(rabbitmq_server_ip);
        consumer_map.insert({rabbitmq_server_ip,
                             std::shared_ptr<Rabbitmq_consumer_pool<N>>(rp)});
    }
    return rp;
}

template <int N>
Rabbitmq_consumer_pool<N>::Rabbitmq_consumer_pool(
    const std::string& rabbitmq_server_ip)
    : m_channel_num(N), m_rabbitmq_server_ip(rabbitmq_server_ip)
{
    m_send_pchannel = AmqpClient::Channel::Create(m_rabbitmq_server_ip);
    for (int i = 0; i < m_channel_num; i++)
    {
        AmqpClient::Channel::ptr_t pchannel
            = AmqpClient::Channel::Create(m_rabbitmq_server_ip);
        std::string consumer_name = CONSUMER_NAME + std::to_string(i);
        std::string queue_name = pchannel->DeclareQueue(consumer_name);
        m_queue_name_vec.push_back(queue_name);
        std::string consumer = pchannel->BasicConsume(queue_name);
        std::thread(&Rabbitmq_consumer_pool::receive_respond_msg, this, std::ref(pchannel), std::ref(consumer)) .detach();
    }
}

template <int N>
std::shared_ptr<Message_completion>
Rabbitmq_consumer_pool<N>::find_and_remove_completion(int msg_id) {
    std::shared_ptr<Message_completion> pc(nullptr);
    {
        std::lock_guard<std::mutex> __locker(m_completions_map_lck);
        auto it = m_completions_map.find(msg_id);
        if (it != m_completions_map.end()) {
            pc = it->second;
            m_completions_map.erase(it);
        }
    }
    return pc;
}

template <int N>
void Rabbitmq_consumer_pool<N>::new_and_add_completion(int msg_id, msg_callback_fun cb, void* arg)
{
    std::shared_ptr<Message_completion> pc(new Message_completion);
    if (0 == pc.get()) {
        // throw error_exception("alloc Message_completion fail",
        // ALLOCATE_FAIL);
    }
    pc->pfunc = cb;
    pc->arg = arg;
    std::lock_guard<std::mutex> __locker(m_completions_map_lck);
    m_completions_map.insert({msg_id, pc});
    return;
}

template <int N>
void Rabbitmq_consumer_pool<N>::receive_respond_msg(
    AmqpClient::Channel::ptr_t& pchannel, std::string& consumer) {
    for (;;) {
        AmqpClient::Envelope::ptr_t envelope = pchannel->BasicConsumeMessage(consumer);
        std::string message_id = envelope->Message()->MessageId();
        int msg_id = atoi(message_id.c_str());
        std::shared_ptr<std::string> pMsg(new std::string(envelope->Message()->Body()));
        std::shared_ptr<Message_completion> pCompletion = find_and_remove_completion(msg_id);
        if (nullptr != pCompletion) {
            pCompletion->pfunc(pMsg, pCompletion->arg);
        }
    }
}

/*
 * sonsumer_pool的send_msg
 */

template< int N>
void Rabbitmq_consumer_pool<N>::send_msg(const std::string& target_exchange,

                                         onst std::string& target_mod,

                                         onst std::string& msg,

                                         sg_callback_fun cb,
 oid* arg)
 

    tatic std::atomic_uint_fast32_t cnt(0);

    nt consumer_index = (cnt++)% m _channel_num;

    mqpClient::BasicMessage::ptr_t message
  
        mqpClient::BasicMessage::Create(msg);

    nt msg_id = generate_msg_id();

    onst std::string message_id = std::to_string(msg_id);
    message->ReplyTo(m_queue_name_vec[consumer_index]);

    essage->MessageId(message_id);

    ew_and_add_completion(msg_id, cb, arg);

    _send_pchannel->BasicPublish(target_exchange, target_mod, message);
}

/*
 * 建立一个consumer_pool的实例 调用 sonsumer_pool 的 send_msg
 */
static void send_msg(const std::string& target_exchange,
					const
                     string& target_mod,
					const string& msg,
					msg_c
                     k_fun cb,
					void* {
	Rabbitm _
    nsumer_pool<RABBITMQ_CONSUMER_NUM>::instance(g_rabbitmq_
        server_ip)->send_msg(
        target_exchange, target_mod, msg, cb, arg);
}

template<int N>
Rab bitmq_consumer_pool<N>::~Rabbitmq_consumer_pool()
{
}



cla sR

tmq_sender
{

public: 	
   bbitmq_
    nder(const std::string& target_addr, int port);
	virtual ~
    bbitmq_sender();
    int send_msg_no_reply(const std::string& target_mod, const std:
                          string& msg);
    int send_msg_sync(const std::string& target_mod, const std::string& msg_s, std::share
                      _ptr<std::string>& msg_reply);
    int send_msg_async(const std::string& target_mod, const std::string& msg_s, msg_callba
                       k_fun cb, void* arg) ;
private: 

     std::string m_exchange;
    int m_target_port;
};


/*******

*******************************************************************
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

Rabbitmq_sender::Rabbitmq_sender(const std::string& target_addr, int port)
    :m_exchange(targ et_addr),m_target_port(p ort)
{
	return;
}

 a
    itmq_sender::~Rabbitmq_sender()
{
}

struct Sy csend_arg
{
	std::condit o
    variable m_cond_var;
	bool complete
    	std::shared_p
    <std::string> p_reply_msg;
	std::mutex lc
    
};
void static sync_send_completion(std::shared_ptr<std::string> pMsg, void* arg)
{
	Sync_send_a g
    sc= static_cast<S ync_send_arg*>(arg);
	std::unique_l
    k < std::mutex >_ocker(sc->ck);
	sc->complete 
    true;
	sc->p_reply_m
     = pMsg;
	sc->m_cond_va
    notify_one();
}

int Rabbitmq_sender::send_msg_no_reply(const std::string& target_mod, const std::str
                                       ng& msg) {
   	send_msg(m
    ange,target_mod, msg , nullptr, nullptr);
    return 0;
}

int Rabbitmq_sender::send_msg_sync(const std::string& target_mod, const std::str
                                   ng& msg_s, std::shared_pt
                                   <std::string>& msg_reply) {
	Sync_send_arg
    rg;
	arg.complete 
    false;
	send_msg(m_ex
    ange, target_mod, msg_s, sync_send_completion, &arg);
	std::unique_l
    k < std::mutex >_ocker(arg.ck);
	while (!arg.c
    plete)
	{
		arg.m_con v
        wait(_locker);
	}
	msg_reply 
    a
    .p_reply_msg;
    return 0;
}

int Rabbitmq_sender::send_msg_async(const std::string& target_mod,
                                    const std::string& msg_s, msg_callback_f
                                    n cb,
               end_msg(m_ x
    ange, target_mod, msg_s, cb, arg);
    return 0;
}
