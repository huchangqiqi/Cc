/*
* XXX This sample code was once meant to show how to use the basic Libevent
* interfaces, but it never worked on non-Unix platforms, and some of the
* interfaces have changed since it was first written.  It should probably
* be removed or replaced with something better.
*
* Compile with:
* cc -I/usr/local/include -o time-test time-test.c -L/usr/local/lib -levent
*/

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <sys/queue.h>
#include <unistd.h>
#endif
#include <time.h>
#ifdef _EVENT_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include<event2/event-config.h>
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>

#ifdef WIN32
#include <winsock2.h>
#endif

int main(int argc, char* argv[])
{
	receiver* rcv = new socket_receiver("10.9.5.113", "5432");
	rcv.start();
	rcv.register_module(1, cb);
	rcv.replay_msg(msg, len, xxx);
	return 0;
}
