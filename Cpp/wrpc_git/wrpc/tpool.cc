#include "rabbitmq_channel_pool.hpp"

int main()
{
    c_ptr pchannel = get_rb_channel();
    AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create("helllooo");
    pchannel->BasicPublish("10.0.0.1", "vm", message);
	return 0;
}
