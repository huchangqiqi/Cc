#include <SimpleAmqpClient/SimpleAmqpClient.h>
class Publisher{
 public:
    Publisher(const std::string& target_ip);
    ~Publisher();
    int publish(const std::string& target_mod,const std::string& msg);
 private:
    AmqpClient::Channel::ptr_t m_channel;
    std::string m_exchange;
    std::string m_correlationId;
};
