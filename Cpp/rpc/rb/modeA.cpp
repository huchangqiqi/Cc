#include <iostream>
#include <fstream>
#include "rabbitmq_sender.h"
void cb(const char* msg, int len, void* arg) {
    std::ofstream fout("test.txt");
    if (fout.is_open()){
        fout << msg;
        fout.close();
    }
    //std::cout << "cb   " <<msg  <<std::endl;
}
int main() {
    Rabbitmq_sender* rb_sender = new Rabbitmq_sender("10.0.0.1", 111);
    const char* msg = "helloo";

    /*
    rb_sender->send_msg_no_reply("vm", msg);
    std::string msg_r ;
    rb_sender->send_msg_sync("vm", msg, msg_r);
    std::cout << msg_r << std::endl;
    */

    const char* arg = "a";
    rb_sender->send_msg_async("vm", msg, cb, (void*)arg);
}
