#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <list>
#include <memory>
#include <mutex>

typedef AmqpClient::Channel::ptr_t c_ptr;
class Rabbitmq_channel_pool
{
  public:
    static Rabbitmq_channel_pool *get_instance();
    auto get_connect() -> c_ptr;
    auto ret_connect(c_ptr &ret) -> void;
    ~Rabbitmq_channel_pool();

  private:
    Rabbitmq_channel_pool(int size);
    auto init_pool(int size) -> void;
    auto destory_pool() -> void;
    auto add_conn(int size) -> void;

    //存放所有连接
    std::list<c_ptr> m_channel_list;

    //连接池对象
    static Rabbitmq_channel_pool *pool ;
    std::mutex m_channel_list_lck;
};

Rabbitmq_channel_pool::Rabbitmq_channel_pool(int size)
{
    init_pool(size);
}

Rabbitmq_channel_pool::~Rabbitmq_channel_pool()
{
}

//初始化连接池
void Rabbitmq_channel_pool::init_pool(int size)
{
    std::lock_guard<std::mutex> _locker(m_channel_list_lck);
    add_conn(size);
}


// 往连接池里面添加连接
void Rabbitmq_channel_pool::add_conn(int size)
{
    for (int i = 0; i < size; ++i) {
        AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("10.9.5.31");
        m_channel_list.push_back(channel);
    }
}
Rabbitmq_channel_pool * Rabbitmq_channel_pool::pool = nullptr;
//获得连接池实例
Rabbitmq_channel_pool *Rabbitmq_channel_pool::get_instance()
{
    if (pool == nullptr) {
        pool = new Rabbitmq_channel_pool(5);
    }
    return pool;
}

//得到一个连接
c_ptr Rabbitmq_channel_pool::get_connect()
{
    std::lock_guard<std::mutex> _locker(m_channel_list_lck);
    c_ptr pchannel = m_channel_list.front();
    m_channel_list.pop_front();
    return pchannel;
}

// 归还一个连接
void Rabbitmq_channel_pool::ret_connect(c_ptr &ret)
{
    std::lock_guard<std::mutex> _locker(m_channel_list_lck);
    m_channel_list.push_back(ret);
}

// // 销毁连接池
// void Rabbitmq_channel_pool::destory_pool()
// {
//     for(auto &channel : m_channel_list)
//         {
//             c_ptr &&sp = std::move(m_channel_list.front());
//             //des sp
//         }
// }
