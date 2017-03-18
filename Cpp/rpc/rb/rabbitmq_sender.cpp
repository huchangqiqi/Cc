#include "rabbitmq_sender.h"
#include <atomic>
#include <functional>
#include <iostream>
#include <thread>

Rabbitmq_sender::Rabbitmq_sender(const std::string& target_addr, int port)
    : Sender(target_addr, port),
      m_channel(AmqpClient::Channel::Create("10.9.5.31")),
      m_exchange(target_addr),
      m_cid("send_one") {
    m_channel->DeclareExchange(m_exchange,
                               AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
    m_reply_queue_name = m_channel->DeclareQueue("");
    m_consumer = m_channel->BasicConsume(m_reply_queue_name);
    /*
      std::thread reply_thread(wait_for_reply,this);
      reply_thread.detach();
    */
}
Rabbitmq_sender::~Rabbitmq_sender()
{
}
/* 从 consumer 获取一条消息并处理
 *
 */
void Rabbitmq_sender::wait_for_reply() {
    AmqpClient::Envelope::ptr_t envelope =
        m_channel->BasicConsumeMessage(m_consumer);
    if (envelope->Message()->CorrelationId() == m_cid) {
        const char* msg_reply = (char*)envelope->Message()->Body().c_str();
        int len = envelope->Message()->Body().length();
        int msg_id = std::stoi(envelope->Message()->MessageId());
        std::shared_ptr<Message_completion> pc =
            find_and_remove_completion(msg_id);
        if (0 != pc.get()) {
            pc->pfunc(msg_reply, len, pc->arg);
        }
    }
}


int Rabbitmq_sender::send_msg_no_reply(const std::string& target_mod,
                                       const char* msg) {
    m_routing_key = target_mod;
    AmqpClient::BasicMessage::ptr_t message =
        AmqpClient::BasicMessage::Create(msg);
    m_channel->BasicPublish(m_exchange, m_routing_key, message);
    return 0;
}

int Rabbitmq_sender::send_msg_sync(const std::string& target_mod,
                                   const char* msg_s, std::string& msg_reply) {
    m_routing_key = target_mod;
    AmqpClient::BasicMessage::ptr_t message =
        AmqpClient::BasicMessage::Create(msg_s);
    int msg_id = generate_msg_id();
    const std::string mid = std::to_string(msg_id);
    message->CorrelationId(m_cid);
    message->ReplyTo(m_reply_queue_name);
    message->MessageId(mid);

    m_channel->BasicPublish(m_exchange, m_routing_key, message);

    for (;;) {
        AmqpClient::Envelope::ptr_t envelope =
            m_channel->BasicConsumeMessage(m_consumer);
        if (envelope->Message()->CorrelationId() == message->CorrelationId()) {
            msg_reply = envelope->Message()->Body();
            break;
        }
    }
    return 0;
}

int Rabbitmq_sender::send_msg_async(const std::string& target_mod,
                                    const char* msg_s, msg_callback_fun cb,
                                    void* arg) {
    m_routing_key = target_mod;
    AmqpClient::BasicMessage::ptr_t message =
        AmqpClient::BasicMessage::Create(msg_s);
    int msg_id = generate_msg_id();
    const std::string mid = std::to_string(msg_id);
    message->CorrelationId(m_cid);
    message->ReplyTo(m_reply_queue_name);
    message->MessageId(mid);
    m_channel->BasicPublish(m_exchange, m_routing_key, message);

    // add_completion
    new_and_add_completion(msg_id, cb, arg);

    // waiting for reply
    //std::thread reply_thread(std::mem_fn(&Rabbitmq_sender::wait_for_reply), this);
    //std::cout << "send_msg_async" << std::endl;
    //reply_thread.join();


    return 0;
}
