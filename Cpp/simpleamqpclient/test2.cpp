#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>

using namespace AmqpClient;

int main(int argc, char *argv[]){
  Channel::ptr_t channel = Channel::Create("localhost");
  //std::string queue = channel->DeclareQueue("test_queue");
  /*
  channel->DeclareExchange("topic_logs",
                        Channel::EXCHANGE_TYPE_TOPIC);
  for(int i=1;i< argc;i++){
    std::string bindingkey = argv[i];
    channel->BindQueue(queue, "topic_logs",bindingkey);
  }
  */

  channel->DeclareExchange("declare_direct_a",
                           Channel::EXCHANGE_TYPE_DIRECT);
  channel->DeclareExchange("declare_direct_b",
                           Channel::EXCHANGE_TYPE_DIRECT);
  std::string bindingkey = "rk";
  std::string queue = channel->DeclareQueue("");
  channel->BindQueue(queue, "declare_direct",bindingkey);

  std::string consumer = channel->BasicConsume(queue);
  for(;;){
      Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer);
      std::cout << envelope->Exchange() << std::endl;
      std::cout << envelope->RoutingKey() << std::endl;
      std::cout << envelope->Message()->Body() << std::endl;
      std::cout << envelope->Message()->CorrelationId() << std::endl;

      BasicMessage::ptr_t message = BasicMessage::Create("rent message...");
      channel->BasicPublish("",
                            "",
                            message);

  }
}
