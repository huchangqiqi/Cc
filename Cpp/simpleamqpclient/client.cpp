#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>

using namespace AmqpClient;

int main(){

  Channel::ptr_t channel = Channel::Create("localhost");
  BasicMessage::ptr_t message = BasicMessage::Create("message body");
  for(int i = 0; i < 20;i++){
    channel->BasicPublish( "declare_direct",
                           "send_queue",
                           message);
    std::cout << "sent " << std::endl;
  }
}
