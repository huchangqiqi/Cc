#include "./rapidjson/include/rapidjson/document.h"
#include "./rapidjson/include/rapidjson/stringbuffer.h"
#include "./rapidjson/include/rapidjson/writer.h"

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <uuid/uuid.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <utility>

using namespace AmqpClient;
int main(int argc, char *argv[]) {
    Channel::ptr_t channel = Channel::Create("10.9.5.31");
    channel->DeclareExchange("direct_logs",
                             AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
    std::string routingkey = "vm";

    /*
    uuid_t uuid;
    char cid[36];
    uuid_generate_random(uuid);
    uuid_unparse(uuid,cid);
    */
    const std::string cid = "1000";
    const std::string reply_queue_name = channel->DeclareQueue("");
    std::string consumer = channel->BasicConsume(reply_queue_name);

    BasicMessage::ptr_t message = BasicMessage::Create(" \
   {\
    \"operation\":\"delete\",\
	\"detail\":{\
    \"uuid\":\"53348393-9e6d-c380-e7ef-80c85b9a3420\" \
               }\
   }\
    ");
    message->CorrelationId(cid);
    message->ReplyTo(reply_queue_name);
    channel->BasicPublish("direct_logs", routingkey, message);

    std::cout << "start consuming" << std::endl;
    for (;;) {
        Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer);
        if (envelope->Message()->CorrelationId() == message->CorrelationId()) {
            std::cout << envelope->Message()->Body() << std::endl;
            break;
        }
    }
    return 0;
}

/*
  void consume(Channel::ptr_t& channel,BasicMessage::ptr_t& message ,std::string
  &
  consumer)
  {
  std::cout << "start consuming" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(30));
  std::cout << "sleep over" << std::endl;
  for(;;){
  Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer);
  if(envelope->Message()->CorrelationId() == message->CorrelationId()){
  std::cout << envelope->Message()->Body() << std::endl;
  break;
  }
  }

  }
*/
/*
  std::thread
  thrd(consume,std::ref(channel),std::ref(message),std::ref(consumer));
  thrd.join();
*/
