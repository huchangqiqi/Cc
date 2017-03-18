#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <string>

class Consumer{
 public:
    Consumer(const std::string& ip_addr);
    ~Consumer();
int start();
 private:
    AmqpClient::Channel::ptr_t m_channel;
    std::string m_exchange;
    std::string m_queue;
    std::string m_consumer;
    std::string m_correlationId;
};
