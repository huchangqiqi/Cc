#include "rabbitmq_sender.hpp"

int main()
{
    Rabbitmq_sender* rb = new Rabbitmq_sender("12",111);
    rb->send_msg_no_reply("vm","hello");
}
