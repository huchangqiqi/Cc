#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>

using namespace AmqpClient;

int main(int argc, char *argv[]){
  Channel::ptr_t channel = Channel::Create("localhost");
  //channel->DeclareExchange("ex_direct",
  //                         Channel::EXCHANGE_TYPE_DIRECT);
  //std::string bindingkey = "rk";
  //channel->BindQueue(queue, "ex_direct",bindingkey);

  std::string queue = channel->DeclareQueue("rpc_queue");
  std::string consumer = channel->BasicConsume(queue);

  for(;;){
      Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer);
      std::cout << envelope->Exchange() << std::endl;
      std::cout << envelope->RoutingKey() << std::endl;
      std::cout << envelope->Message()->Body() << std::endl;
      std::cout << envelope->Message()->CorrelationId() << std::endl;
      std::cout << envelope->Message()->ReplyTo() << std::endl;

      std::cout << "start reply" << std::endl;
      BasicMessage::ptr_t message = BasicMessage::Create("reply message.");
      message->CorrelationId(envelope->Message()->CorrelationId());

      std::string routingkey = envelope->Message()->ReplyTo();
      channel->BasicPublish("",
                            routingkey,
                            message);

  }
}
