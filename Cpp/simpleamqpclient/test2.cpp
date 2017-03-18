#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>

using namespace AmqpClient;

int main(int argc, char *argv[]){
  Channel::ptr_t channel = Channel::Create("localhost");
  channel->DeclareExchange("mod2_ip",AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
  std::string queue = channel->DeclareQueue("");
  std::string bindingkey = "mod2";
  channel->BindQueue(queue,"mod2_ip",bindingkey);

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
