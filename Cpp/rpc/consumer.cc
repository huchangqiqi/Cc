#include "consumer.h"
#include <iostream>
#include <fstream>
#include <thread>
Consumer::Consumer(const std::string& ip_addr)
    :m_channel(AmqpClient::Channel::Create("10.9.5.31")), m_exchange(ip_addr) ,m_correlationId("s1")
{
    m_channel->DeclareExchange(m_exchange, AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
    m_queue = m_channel->DeclareQueue("");
    m_channel->BindQueue(m_queue,m_exchange,"reply");
    m_consumer = m_channel->BasicConsume(m_queue);
}
Consumer::~Consumer(){}
int Consumer::start()
{
    for(;;){
        AmqpClient::Envelope::ptr_t envelope =
            m_channel->BasicConsumeMessage(m_consumer);
        if (envelope->Message()->CorrelationId() == m_correlationId) {
            std::ofstream fout("test.txt");
            if (fout.is_open()){
                fout << envelope->Message()->Body();
                fout.close();
            }
        }
        break;
    }
    return 0;
}

