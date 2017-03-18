#include "publisher.h"
Publisher::Publisher(const std::string& target_ip)
    :m_channel(AmqpClient::Channel::Create("10.9.5.31")),
     m_exchange(target_ip),
     m_correlationId("s1")
{
    m_channel->DeclareExchange(m_exchange,
                               AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
}
Publisher::~Publisher(){}
int Publisher::publish( const std::string &target_mod, const std::string &msg )
{
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create(msg);
    message->CorrelationId(m_correlationId);
    message->MessageId("101");
    // exchange, routing_key, msg
    m_channel->BasicPublish(m_exchange,target_mod,message );
    return 0;
}
