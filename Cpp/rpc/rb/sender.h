#pragma once
#ifndef __SENDER_H__
#define __SENDER_H__
#include <string>
#include <mutex>
#include <memory>
#include <atomic>
#include <event2/event.h>
#include <unordered_map>
typedef void(*msg_callback_fun)(const char* msg, int len, void* arg);

typedef struct Msg_completion_str
{
	msg_callback_fun pfunc;
	void* arg;
}Message_completion;

class Sender
{
public:
	Sender(const std::string& target_ip, int target_port)
		:m_target_ip(target_ip), m_target_port(target_port)
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
	virtual int send_msg_no_reply(const std::string& targe_mod, const char* msg) = 0;
	virtual int send_msg_sync(const std::string& target_mod, const char* msg_s,std::string& msg_reply) = 0;
	virtual int send_msg_async(const std::string& target_mod,const char* msg_s, msg_callback_fun cb, void* arg) = 0;
	virtual ~Sender(){return;};
protected:
	int new_and_add_completion(int msg_id, msg_callback_fun cb, void* arg)
	{
		std::shared_ptr<Message_completion> pc(new Message_completion);
		pc->pfunc = cb;
		pc->arg = arg;
		std::lock_guard<std::mutex> __locker(m_locker);
		m_completions.insert({ msg_id, pc});
        return 0;
	}

	std::shared_ptr<Message_completion> find_and_remove_completion(int msg_id)
	{
		std::shared_ptr<Message_completion> pc(nullptr);
		{
			std::lock_guard<std::mutex> __locker(m_locker);
			std::unordered_map<int, std::shared_ptr<Message_completion>>::iterator it = m_completions.find(msg_id);
			if (it != m_completions.end())
            {
                pc = it->second;
				m_completions.erase(it);
			}
		}
		return pc;
	}

	int generate_msg_id()
	{
		//static std::atomic_uint64_t id = 0;
        static int id = 1;
		return id++;
	}

private:
	std::mutex m_locker;
	std::string m_target_ip;
	std::unordered_map<int, std::shared_ptr<Message_completion>> m_completions;
	int m_target_port;
};
#endif // !__SENDER_H__
