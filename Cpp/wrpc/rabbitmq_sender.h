#pragma once
#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "sender.h"
class Rabbitmq_sender : public sender
{
public:
	Rabbitmq_sender(const std::string& target_addr, int port);
	virtual ~Rabbitmq_sender();
	virtual int send_msg_no_reply(int target_mod, std::string& msg, int len);
	virtual int send_msg_sync(int target_mod, std::string& msg_s, int len_s,
                            std::string& msg_replay, int len_r);
	virtual int send_msg_async(int target_mod, std::string& msg_s, int len_s,
                             msg_callback_fun cb, void* arg) ;
 private:
  std::string m_exchange;
  std::string  m_routing_key;
  AmqpClient::Channel::ptr_t m_channel;
};

