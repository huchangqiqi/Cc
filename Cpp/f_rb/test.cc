#include "rabbitmq_sender.hpp"
#include <iostream>
int main()
{
    rabbitmq_sender_init("10.9.5.31",
                         "10.0.0.1",
                         1001,
                         "exchange_for_sender",
                         "exchange_for_receiver");
    Rabbitmq_sender* rb_send = new Rabbitmq_sender("10.0.0.2",1002);
    std::shared_ptr<std::string> msg_reply ;
    rb_send->send_msg_sync("vm", "hiii", msg_reply);
    std::cout << *msg_reply.get() << std::endl;
}
