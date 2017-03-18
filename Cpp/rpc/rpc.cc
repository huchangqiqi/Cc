#include "rpc.h"
#include <iostream>
#include <thread>

/* consumer 通过 ip_addr
 * publisher 通过 target_ip
 */
RPC::RPC(const std::string& ip_addr, const std::string& target_ip)
    : m_consumer(ip_addr), m_publisher(target_ip) {}
RPC::~RPC() {}
void RPC::call(const std::string& msg) {
    // cousumer 监听消息 in a thread
    // std::thread t(std::mem_fn(&Consumer::start), this->m_consumer);
    // publisher 发送消息  target_mod msg
    m_publisher.publish("vm", msg);
    m_consumer.start();
}
void RPC::call_f(const std::string& msg) {
    std::thread call_thread(std::mem_fn(&RPC::call),this,std::ref(msg));

    call_thread.join();
}
