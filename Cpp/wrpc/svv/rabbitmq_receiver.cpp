#include "rabbitmq_receiver.h"
#include <iostream>

Rabbitmq_receiver::Rabbitmq_receiver(const std::string& ip_addr, int port,
                                     int mode_id)
{
  m_channel = AmqpClient::Channel::Create("localhost");
  m_exchange = ip_addr;
  m_channel->DeclareExchange(m_exchange,AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
  m_queue = m_channel->DeclareQueue("");
  std::string binding_key = std::to_string(mode_id);; //mod_id
  m_channel->BindQueue(m_queue,m_exchange,binding_key);
  m_consumer = m_channel->BasicConsume(m_queue);

}

Rabbitmq_receiver::~Rabbitmq_receiver()
{

}

int Rabbitmq_receiver::start()
{
  for(;;){

     m_envelope = m_channel->BasicConsumeMessage(m_consumer);
     std::cout << m_envelope->Message()->Body() << std::endl;
     AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create("rrrr");
     message->CorrelationId(m_envelope->Message()->CorrelationId());
     m_channel->BasicPublish("",
                             m_envelope->Message()->ReplyTo(),
                             message);

     }
  return 0;
}

void Rabbitmq_receiver::message_reply(std::string& msg_r, int len, void* arg)
{
  std::cout << "reply" << std::endl;
  AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create(msg_r);
  message->CorrelationId(m_envelope->Message()->CorrelationId());
  m_channel->BasicPublish("",
                        m_envelope->Message()->ReplyTo(),
                        message);
}



