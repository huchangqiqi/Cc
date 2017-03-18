#include "rabbitmq_sender.hpp"

int main() {

	Rabbitmq_sender *rb_sender = new Rabbitmq_sender("10.0.0.1",998);
	std::shared_ptr<std::string> msg_reply;
	for(int i =0; i< 10; i++){
		rb_sender->send_msg_sync("vm", "hellooo", msg_reply);
		std::cout << *msg_reply.get() << std::endl;
	}
	return 0;
}
