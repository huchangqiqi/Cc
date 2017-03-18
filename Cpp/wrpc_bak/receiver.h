#ifndef __RECEIVER_H__
#define __RECEIVER_H__

#include <string>
#include <map>
typedef void(*msg_receive_func)(char* msg_r, int len, void* arg);

class receiver
{
public:
	receiver(const std::string& ip_addr, int port)
		:m_ip_addr(ip_addr), m_port(port)
	{
		return;
	}
	virtual ~receiver() { return; };
	virtual int start() = 0;
	int register_module(int module_id, msg_receive_func) 
	{
		if (nullptr == m_modules[module_id])
		{
			m_modules[module_id] = msg_receive_func;
		}
		else
		{
			return -1;
		}
	}
	void unregister_module(int module_id)
	{
		if (nullptr != m_modules.find(module_id))
		{
			m_modules.clear(module_id);
		}
	}
	virtual void message_replay(char* msg_r, int len, void* arg) = 0;
protected:
	int duplex_msg(int module_id, char* msg, int len, void* arg)
	{
		if (nullptr != m_modules.find[module_id])
		{
			m_modules[module_id](msg, len, arg);
		}
	}
private:
	std::string m_ip_addr;
	int m_port;
	std::map<int, msg_receive_func> m_modules;
};
#endif // !__RECEIVER_H__