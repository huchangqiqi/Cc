#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <amqp.h>
#include <amqp_framing.h>
#include <iostream>

using namespace AmqpClient;

int main(int argc,char *argv[]){
  Channel::ptr_t channel = Channel::Create("localhost");
  std::string request_queue_name = "rpc_queue";
  const std::string cid = "1233";
  const std::string reply_queue_name = channel->DeclareQueue("");
  std::string consumer = channel->BasicConsume(reply_queue_name);

  BasicMessage::ptr_t message =  BasicMessage::Create("Message body!!!");
  message->CorrelationId(cid);
  message->ReplyTo( reply_queue_name );

  channel->BasicPublish("",
                        request_queue_name,
                        message);

  std::cout << "sent " << request_queue_name << std::endl;

  std::cout << "start envelope" << std::endl;
  for(;;){
    Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer);
    if(envelope->Message()->CorrelationId() == message->CorrelationId()){
      std::cout << envelope->Message()->CorrelationId() << std::endl;
      std::cout << envelope->Message()->Body() << std::endl;
      break;
      }
  }
}
