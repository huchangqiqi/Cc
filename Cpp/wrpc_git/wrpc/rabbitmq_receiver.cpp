#include "rabbitmq_receiver.h"
#include <iostream>

typedef struct Msg_str {
    std::string cid;
    std::string reply_to;
} Msg;
Rabbitmq_receiver::Rabbitmq_receiver(const std::string& ip_addr, int port,
                                     int module_id, msg_receive_func pfunc)
    : Receiver(ip_addr, port) {
    m_module_id = module_id;
    register_module(module_id, pfunc);
    m_channel = AmqpClient::Channel::Create("localhost");
    m_exchange = ip_addr;
    m_channel->DeclareExchange(m_exchange,
                               AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
    m_queue = m_channel->DeclareQueue("");
    std::string binding_key = std::to_string(module_id);  // mod_id
    m_channel->BindQueue(m_queue, m_exchange, binding_key);
    m_consumer = m_channel->BasicConsume(m_queue);
}
Rabbitmq_receiver::~Rabbitmq_receiver() {}

int Rabbitmq_receiver::start() {
    for (;;) {
        AmqpClient::Envelope::ptr_t envelope =
            m_channel->BasicConsumeMessage(m_consumer);
        const char* msg = envelope->Message()->Body().c_str();
        int len = envelope->Message()->Body().length();
        Msg* msg_p = new Msg();
        msg_p->cid = envelope->Message()->CorrelationId();
        msg_p->reply_to = envelope->Message()->ReplyTo();
        duplex_msg(m_module_id, msg, len, msg_p);
        message_reply("reply_messagestart",20,msg_p);
    }
    return 0;
}

void Rabbitmq_receiver::message_reply(const char* msg_r, int len, void* arg) {
    std::cout << "reply" << std::endl;
    AmqpClient::BasicMessage::ptr_t message =
        AmqpClient::BasicMessage::Create(msg_r);
    Msg* msg_p = (Msg*)arg;
    message->CorrelationId(msg_p->cid);
    m_channel->BasicPublish("", msg_p->reply_to, message);
    delete msg_p;
    Receiver::unregister_module(m_module_id);
}
