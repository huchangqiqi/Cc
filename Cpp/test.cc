#include "rpc.hpp"

void cb(const std::shared_ptr<std::string> pMsg, void *arg)
{
    std::cout << *pMsg.get() << std::endl;
}

void send_100(RPC *rpc)
{
    for (int i = 0; i < 5; i++) {
        rpc->send_msg_async("hellllo", "vm", cb, nullptr);
    }
}
int main()
{
    RPC *rpc = new RPC("10.0.0.1", "10.0.0.2", "reply");

    // rpc->send_msg_no_reply("no_reply", "vm");

    // // std::thread ts(send_100,rpc);
    // ts.detach();

    rpc->send_msg_async("hellooo", "vm", cb, nullptr);


    // std::shared_ptr<std::string> msg_reply;
    // rpc->send_msg_sync("hhhhh", "vm", msg_reply);
    // std::cout << *msg_reply.get() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
