#pragma once
#ifndef __SENDER_H__
#define __SENDER_H__
#include<string>
#include <event2/event.h>

typedef void(*msg_callback_fun)(const std::string& msg, int len, void* arg);

class sender
{
public:
	Sender(const std::string& target_ip, int target_port)
		:m_target_ip(target_ip), m_target_port(m_target_port)
	{
		return;
	}
	const std::string& get_target_addr()const
	{
		return m_target_ip;
	}
	int get_target_port()const
	{
		return m_target_port;
	}
	virtual int send_msg_no_reply(int targe_mod, std::string* msg, int len) = 0;
	virtual int send_msg_sync(int target_mod, char* msg_s, int len_s, char* msg_replay, int& len_r) = 0;
	virtual int send_msg_async(int target_mod, char* msg_s, int len_s, msg_callback_fun cb, void* arg) = 0;
	virtual ~sender();
private:
	std::string m_target_ip;
	int m_target_port;
};
#endif // !__SENDER_H__
