#include "receiver.h"
#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

class Rabbitmq_receiver : public Receiver
{
public:
    Rabbitmq_receiver(const std::string& ip_addr,int port,int module_id,msg_receive_func pfunc);
    virtual ~Rabbitmq_receiver();
    virtual int start();
    virtual void message_reply(const char* msg_r,int len, void *arg);
private:
    int m_module_id;
    std::string m_exchange;
    std::string m_queue;
    std::string m_consumer;
    AmqpClient::Channel::ptr_t m_channel;
};

