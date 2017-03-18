#ifndef __RECEIVER_H__
#define __RECEIVER_H__
#include <mutex>
#include <string>
#include <unordered_map>
typedef void(*msg_receive_func)(const void* msg_r, int len, void* arg);

class Receiver
{
public:
	Receiver(const std::string& ip_addr, int port)
		:m_ip_addr(ip_addr), m_port(port)
	{
		return;
	}
	virtual ~Receiver() { return; };
	virtual int start() = 0;
	virtual void message_reply(const char* msg_r, int len, void* arg) = 0;
	int register_module(int module_id, msg_receive_func pfunc)
	{
		std::lock_guard<std::mutex> __locker(m_lock_modules);
		if (nullptr == m_modules[module_id])
		{
			m_modules[module_id] = pfunc;
		}
		else
		{
			return -1;
		}
	}
	void unregister_module(int module_id)
	{
		std::lock_guard<std::mutex> __locker(m_lock_modules);
		std::unordered_map<int, msg_receive_func>::iterator it = m_modules.find(module_id);
		if (m_modules.end() != it)
		{
			m_modules.erase(it);
		}
	}
protected:
	int duplex_msg(int module_id, const void* msg, int len, void* arg)
	{
		if (m_modules.end() != m_modules.find(module_id))
		{
			m_modules[module_id](msg, len, arg);
		}
	}
private:
	std::mutex m_lock_modules;
	std::string m_ip_addr;
	int m_port;
	std::unordered_map<int, msg_receive_func> m_modules;
};
#endif // !__RECEIVER_H__
