#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <amqp.h>
#include <amqp_framing.h>
#include <iostream>

struct amqp_basic_properties_t_;
using namespace AmqpClient;

int main(int argc,char *argv[]){
  Channel::ptr_t channel = Channel::Create("localhost");
  /*
  channel->DeclareExchange("topic_logs",
                           Channel::EXCHANGE_TYPE_TOPIC);
  std::string routingkey = argc > 1 ? argv[1] : "anonymous.info";
  */

  channel->DeclareExchange("declare_direct_a",
                           Channel::EXCHANGE_TYPE_DIRECT);
  channel->DeclareExchange("declare_direct_b",
                           Channel::EXCHANGE_TYPE_DIRECT);
  std::string routingkey = "rk";
  BasicMessage::ptr_t message =  BasicMessage::Create("Message body!!!");
  const std::string cid = "1233";
  message->CorrelationId(cid);

  channel->BasicPublish("declare_direct",
                        routingkey,
                        message);
  std::cout << "sent " << routingkey << message << std::endl;

  std::string bindingkey = "rkb";
  std::string queue = channel->DeclareQueue("");
  channel->BindQueue(queue,"",bindingkey);


}
