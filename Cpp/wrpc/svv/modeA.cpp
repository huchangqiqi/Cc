#include "rabbitmq_sender.h"
#include <iostream>

void cb(const std::string& msg,int len, void *arg){
  std::cout << msg << std::endl;
  std::cout << "cb" << std::endl;
}
int main()
{
  Rabbitmq_sender* rb_sender = new Rabbitmq_sender("10.0.0.1",111);
  std::string msg = "hello1";
  //rb_sender->send_msg_no_reply(2, msg, 20);
  //std::string msg_replay ;
  //rb_sender->send_msg_sync( 2,msg ,20, msg_replay, 20);
  rb_sender->send_msg_async(2, msg, 20, cb, NULL );
}
