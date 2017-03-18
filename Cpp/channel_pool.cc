#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <mutex>

template<int N>
class Channel_pool
{
public:
    static Channel_pool<N>* instance(const std::string& rabbitmq_server_ip);
private:
    Channel_pool(){}
    Channel_pool(const std::string& rabbitmq_server_ip);
    int m_channel_num;
    std::vector<AmqpClient::Channel::ptr_t> m_channel_vec;
    std::string m_rabbitmq_server_ip;
};

template<int N>
Channel_pool<N>::Channel_pool(const std::string& rabbitmq_server_ip)
    :m_channel_num(N),
     m_rabbitmq_server_ip(rabbitmq_server_ip)
{
    for(int i = 0; i < m_channel_num; i++)
        {
            AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create(m_rabbitmq_server_ip);
            m_channel_vec.push_back(channel);
        }
}

template<int N>
Channel_pool<N>* Channel_pool<N>::instance( const std::string &rabbitmq_server_ip )
{
    static std::unordered_map<std::string, std::shared_ptr<Channel_pool<N>>> channel_map;
    static std::mutex channel_map_lck;
    Channel_pool* channel_pool_ptr;
    std::lock_guard<std::mutex> __lock_channel_map(channel_map_lck);
    auto itor = channel_map.find(rabbitmq_server_ip);
    if(channel_map.end() != itor)
        {
            channel_pool_ptr = itor->second.get();
        }
    else
        {
            channel_pool_ptr = new Channel_pool(rabbitmq_server_ip);
            channel_map.insert({rabbitmq_server_ip,
                        std::shared_ptr<Channel_pool<N>>(channel_pool_ptr)});
        }
    return channel_pool_ptr;
}
