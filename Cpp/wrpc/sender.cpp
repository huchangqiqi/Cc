#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "sender.h"
sender::sender(struct event_base* base, const std::string& target_ip, int target_port)
{
	//初始化远端地址
	struct sockaddr_in target_addr;
	memset(&target_addr, 0, sizeof(target_addr));
	target_addr.sin_family = AF_INET;
	target_addr.sin_addr.s_addr = htonl(inet_addr(target_ip.c_str()));
	target_addr.sin_port = htons(target_port);
	//生成socket_fd
	m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	m_connection = bufferevent_socket_new(base, m_socket_fd, BEV_OPT_CLOSE_ON_FREE);
	if (NULL == m_connection)
	{
		throw std::runtime_error("invoke bufferevent_socket_new failed");
	}
	char enable = 1;
	if (setsockopt(m_socket_fd, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable)) < 0)
	{
		bufferevent_free(m_connection);
		throw std::runtime_error("invoke setsockopt failed");
	}
	if(-1 == bufferevent_enable(m_connection, EV_WRITE|EV_READ))
	{
		bufferevent_free(m_connection);
		throw std::runtime_error("invoke bufferevent_enable failed");
	}
	//与远端服务器建立连接
	if (bufferevent_socket_connect(m_connection, (struct sockaddr*)&target_addr, sizeof(target_addr)) != 0)
	{
		bufferevent_free(m_connection);
		throw std::runtime_error("invoke bufferevent_socket_connect failed");
	}
}

sender::~sender()
{
	bufferevent_free(m_connection);
}


int sender::send_msg_no_reply(int targe_mod, char* msg, int len)
{
	int ret;
	if (-1 == (ret = bufferevent_write(m_connection, static_cast<void*>(msg), len)))
	{
		throw std::runtime_error("send message error");
	}
	return ret;
}

int sender::send_msg_sync(int target_mod, char* msg_s, int len_s, char* msg_r, int& len_r)
{
	comp = new xxx_completion();
	send_msg_async()
	{
		wait_on_competion(comp);
	}
	msg_r = xxx;
	len_r = xxx;
	return;
	//waiting untill msg return	
}
int sender::send_msg_async(int target_mod, char* msg_s, int len_s, msg_callback_fun cb)
{
	if (-1 == bufferevent_write(m_connection, static_cast<void*>(msg_s), len_s))
	{
		throw std::runtime_error("send message error");
	}
}
