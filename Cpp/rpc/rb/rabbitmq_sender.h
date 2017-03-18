#pragma once
#include <string>
#include <thread>
#include <iostream>
#include "sender.h"
#include "SimpleAmqpClient/SimpleAmqpClient.h"
class Rabbitmq_sender : public Sender
{
public:
    Rabbitmq_sender(const std::string& target_addr, int port);
	virtual ~Rabbitmq_sender();
	virtual int send_msg_no_reply(const std::string& targe_mod, const char* msg);
	virtual int send_msg_sync(const std::string& target_mod, const char* msg_s,std::string& msg_reply);
	virtual int send_msg_async(const std::string& target_mod,const char* msg_s, msg_callback_fun cb, void* arg);
    void wait_for_reply();
   private:
    AmqpClient::Channel::ptr_t m_channel;
    std::string m_routing_key;
    std::string m_exchange;
    std::string m_cid;
    std::string m_reply_queue_name;
    std::string m_consumer;
};

