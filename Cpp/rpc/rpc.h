#pragma once
#include "publisher.h"
#include "consumer.h"
#include <string>
#include <thread>
class RPC
{
public:
    RPC(const std::string& ip_addr, const std::string& target_ip);
    ~RPC();
    void call(const std::string& msg);
    void call_f(const std::string& msg);
    void cast(const std::string& msg);
private:
    Consumer m_consumer;
    Publisher m_publisher;
    std::thread call_thread;
};
