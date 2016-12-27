#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>
using namespace AmqpClient;
int main(){

  Channel::ptr_t channel = Channel::Create("localhost");
  channel->DeclareExchange( "declare_direct", Channel::EXCHANGE_TYPE_DIRECT, false, false, false);
  std::cout << "starting server" << std::endl;
  std::string queue = channel->DeclareQueue("send_queue",false,true,false,false);
  channel->BindQueue(queue, "declare_direct");

  std::string consumer  = channel->BasicConsume(queue,"send_queue");
  for(;;){
      Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer);
      std::cout << envelope->Exchange() << std::endl;
      std::cout << envelope->RoutingKey() << std::endl;
      std::cout << envelope->Message()->Body() << std::endl;
  }
  channel->BasicCancel(consumer);

}
