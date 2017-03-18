#include "rabbitmq_channel_pool.hpp"

Rabbitmq_channel_pool *pool = Rabbitmq_channel_pool::get_instance();
int main()
{
    c_ptr pchannel = pool->get_connect();
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create("helllooo");
    pchannel->BasicPublish("10.0.0.1", "vm", message);

}
