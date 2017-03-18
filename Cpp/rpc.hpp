#include "rabbitmq_channel_pool.hpp"
#include "SimpleAmqpClient/SimpleAmqpClient.h"
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>
#include <unordered_map>

typedef void (*msg_callback_fun)(const std::shared_ptr<std::string> pMsg, void *arg);
typedef struct Msg_completion_str {
    msg_callback_fun pfunc;
    void *arg;
} Message_completion;
typedef struct Sync_send_arg_str {
    std::condition_variable m_cond_var;
    bool complete;
    std::shared_ptr<std::string> p_reply_msg;
    std::mutex lck;
} Sync_send_arg;

void static sync_send_completion(std::shared_ptr<std::string> pMsg, void *arg)
{
    Sync_send_arg *sc = static_cast<Sync_send_arg *>(arg);
    std::unique_lock<std::mutex> _locker(sc->lck);
    sc->complete = true;
    sc->p_reply_msg = pMsg;
    sc->m_cond_var.notify_one();
}

int generate_msg_id()
{
    static std::atomic_uint_fast32_t id(0);
    return id++;
}
class Rabbitmq_consume
{
public:
    static Rabbitmq_consume* instance(const std::string& rabbitmq_server_ip);
private:
    Rabbitmq_consume(){}
    Rabbitmq_consume(const std::string& rabbitmq_server_ip);
    std::string m_rabbitmq_server_ip;

};



class RPC
{
  public:
    RPC(const std::string &send_ip, const std::string &reply_ip,const std::string &reply_routingkey);
    void send_msg_no_reply(const std::string &msg, const std::string &target_mod);
    void send_msg_async(const std::string &msg, const std::string &target_mod, msg_callback_fun cb, void *arg);
    void send_msg_sync(const std::string &msg, const std::string &target_mod, std::shared_ptr<std::string> &msg_reply);
    void receive_msg();

  private:
    std::string m_send_ip;
    std::string m_reply_ip;
    AmqpClient::Channel::ptr_t m_r_pchannel;
    AmqpClient::Channel::ptr_t m_s_pchannel;
    std::string m_reply_routingkey;
    std::string m_reply_queue_name;
    std::unordered_map<int, std::shared_ptr<Message_completion>> m_completions_map;
    std::mutex m_completions_map_lck;

    void new_and_add_completion(int msg_id, msg_callback_fun cb, void *arg);
    std::shared_ptr<Message_completion> find_and_remove_completion(int msg_id);
};

/**
 * @name RPC - RPC的构造函数
 * @param send_ip -  send ip 目标模块的ip 作为发送的exchange
 * @param reply_ip -  reply ip 发送RPC请求的模块ip 作为返回的exchange
 * @param rbserver_ip -  rbserver ip Rabbitmq服务器的ip
 * @param reply_routingkey -  reply routingkey 接受返回消息的routingkey
 * @return RPC
 */
RPC::RPC(const std::string &send_ip, const std::string &reply_ip,  const std::string &reply_routingkey)
    : m_send_ip(send_ip), m_reply_ip(reply_ip),
      m_reply_routingkey(reply_routingkey)
{
    Rabbitmq_channel_pool *pool = Rabbitmq_channel_pool::get_instance();
    m_r_pchannel = pool->get_connect();
    m_s_pchannel = pool->get_connect();
    m_reply_queue_name = m_r_pchannel->DeclareQueue("");
    m_r_pchannel->BindQueue(m_reply_queue_name, m_reply_ip, m_reply_routingkey);

    std::thread tr(std::mem_fn(&RPC::receive_msg), this);
    tr.detach();
}

/**
 * @name send_msg_sync - Sends amessage
 * @param msg -  message 发送的请求消息
 * @param target_mod -  target mod 目标模块的模块名
 * @param cb -  cb 处理返回消息的回调函数
 * @param arg - Empty arg 回调函数的参数
 * @return void
 */
void RPC::send_msg_async(const std::string &msg, const std::string &target_mod, msg_callback_fun cb, void *arg)
{
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create(msg);
    int msg_id = generate_msg_id();
    const std::string message_id = std::to_string(msg_id);
    message->MessageId(message_id);
    new_and_add_completion(msg_id, cb, arg);
    m_s_pchannel->BasicPublish(m_send_ip, target_mod, message);
}

void RPC::send_msg_no_reply(const std::string &msg, const std::string &target_mod)
{
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create(msg);
    int msg_id = generate_msg_id();
    const std::string message_id = std::to_string(msg_id);
    message->MessageId(message_id);
    m_s_pchannel->BasicPublish(m_send_ip, target_mod, message);
}

void RPC::send_msg_sync(const std::string &msg, const std::string &target_mod, std::shared_ptr<std::string> &msg_reply)
{
    Sync_send_arg sync_arg;
    sync_arg.complete = false;
    send_msg_async(msg, target_mod, sync_send_completion, &sync_arg);
    std::unique_lock<std::mutex> _locker(sync_arg.lck);
    while (!sync_arg.complete) {
        sync_arg.m_cond_var.wait(_locker);
    }
    msg_reply = sync_arg.p_reply_msg;
}

void RPC::receive_msg()
{
    std::string consumer_tag = m_r_pchannel->BasicConsume(m_reply_queue_name);
    for (;;) {
        AmqpClient::Envelope::ptr_t envelope;
        m_r_pchannel->BasicConsumeMessage(consumer_tag, envelope, 100);
        if (envelope.get() == nullptr) {
            break;
        }
        std::string message_id = envelope->Message()->MessageId();
        int msg_id = atoi(message_id.c_str());
        std::shared_ptr<std::string> pMsg(new std::string(envelope->Message()->Body()));
        std::shared_ptr<Message_completion> pCompletion = find_and_remove_completion(msg_id);
        if (nullptr == pCompletion.get()) {
            break;
        }
        pCompletion->pfunc(pMsg, pCompletion->arg);
    }
}

void RPC::new_and_add_completion(int msg_id, msg_callback_fun cb, void *arg)
{
    std::shared_ptr<Message_completion> pc(new Message_completion);
    if (nullptr == pc.get()) {
        // throw error_exception("alloc Message_completion fail", ALLOCATE_FAIL);
    }
    pc->pfunc = cb;
    pc->arg = arg;
    std::lock_guard<std::mutex> __locker(m_completions_map_lck);
    m_completions_map.insert({msg_id, pc});
    return;
}

std::shared_ptr<Message_completion> RPC::find_and_remove_completion(int msg_id)
{
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
