#include "rabbitmq_receiver.h"

int main() {
    Rabbitmq_receiver* rb_receiver = new Rabbitmq_receiver("10.0.0.1", 111, 2);
    rb_receiver->start();
    /*
    std::string msg_r = "reply message";
    rb_receiver->message_reply(msg_r, 50, NULL);
    */
}
