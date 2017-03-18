#include "receiver.h"
#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

class Rabbitmq_receiver
{
 public:
  Rabbitmq_receiver(const std::string& ip_addr,int port,int mode_id);
  virtual ~Rabbitmq_receiver();
  virtual int start();
  virtual void message_reply(std::string& msg_r,int len, void *arg);
private:
  std::string m_exchange;
  std::string m_queue;
  std::string m_consumer;
  AmqpClient::Channel::ptr_t m_channel;
  AmqpClient::Envelope::ptr_t m_envelope ;
};
