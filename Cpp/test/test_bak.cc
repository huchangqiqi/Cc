#include "rabbitmq_sender.hpp"
#include <iostream>
std::string g_rabbitmq_server_ip = "localhost";
int main()
{

    Rabbitmq_sender* rb_sender = new Rabbitmq_sender("10.0.0.1",111);
    for(int i=0; i<10; i++)
        {
            std::shared_ptr<std::string> msg_reply;
            rb_sender->send_msg_sync("vm", "helloo", msg_reply );
            std::cout << *msg_reply.get() << std::endl;
        }
    return 0;
}
