#include "sender.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>
class rb_sender : public sender
{
 public:
  rb_sender(const std::string& target_ip,int target_port);
	virtual int send_msg_no_reply(int target_mod, char* msg, int len);
	virtual int send_msg_sync(int target_mod, char* msg_s, int len_s, char* msg_replay, int& len_r);
	virtual int send_msg_async(int target_mod, char* msg_s, int len_s, msg_callback_fun cb, void* arg);
	virtual ~rb_sender();
 private:
  std::string m_target_ip;
  int m_target_port;
  AmqpClient::Channel::ptr_t channel;
};
