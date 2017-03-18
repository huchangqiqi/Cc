#include "rb_sender.h"
#include <boost/lexical_cast.hpp>

rb_sender::rb_sender(const std::string& target_ip, int target_prot){
  channel = AmqpClient::Channel::Create("localhost");
  channel->DeclareExchange("ex_direct",AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
}


int rb_sender::send_msg_no_reply( int target_mod, char *msg, int len ){
  AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create(msg);
  std::string routingkey = boost::lexical_cast<std::string>(target_mod);
  const std::string id;
  message->CorrelationId(id);
  channel->BasicPublish("ex_direct", routingkey, message);
  return 0;
}
