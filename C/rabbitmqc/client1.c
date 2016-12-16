#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <amqp_framing.h>

#include "utils.h"

void* send_msg_async(void *ptf)
{
  printf("client1 starting send msg\n");
  char const *hostname;
  int port, status;
  char const *exchange;
  char const *exchangetype;
  char const *routingkey;
  char const *messagebody;
  amqp_socket_t *socket = NULL;
  amqp_connection_state_t conn;

  hostname = "localhost";
  port = atoi("5672");
  exchange = "send";
  exchangetype = "direct";
  routingkey = "send";
  messagebody = ptf;

  conn = amqp_new_connection();
  socket = amqp_tcp_socket_new(conn);
  if(!socket){
    die("creating TCP socket");
  }
  status = amqp_socket_open(socket, hostname,port);
  if(status){
    die("opening TCP socket");
  }
  die_on_amqp_error(amqp_login(conn,"/",0,131072,0,AMQP_SASL_METHOD_PLAIN,"guest","guest"),
                    "Logging in");
  amqp_channel_open(conn,1);
  die_on_amqp_error(amqp_get_rpc_reply(conn),"Opening channel");

  {
    amqp_exchange_declare(conn,1,amqp_cstring_bytes(exchange),amqp_cstring_bytes(exchangetype),
                          0,0,0,0,amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn),"Declaring exchange");

  }

  for(int i = 0;i < 10; i++){

    sleep(1);
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode =2;

    char buff[20];
    snprintf(buff,sizeof(buff),"%d",i);
    props.correlation_id = amqp_cstring_bytes(buff);

    die_on_error(amqp_basic_publish(conn,
                                    1,
                                    amqp_cstring_bytes(exchange),
                                    amqp_cstring_bytes(routingkey),
                                    0,
                                    0,
                                    &props,
                                    amqp_cstring_bytes(messagebody)),
                 "Publishing");

    printf("messagebody = %s\n",ptf);
    printf("published\n");

  }

  die_on_amqp_error(amqp_channel_close(conn,1,AMQP_REPLY_SUCCESS),"Closing channel");
  die_on_amqp_error(amqp_connection_close(conn,AMQP_REPLY_SUCCESS),"Closing connection");
  die_on_error(amqp_destroy_connection(conn),"Ending connection");

}

void* receive(void *ptr)
{
  printf("client1 starting receive\n");
  char const *hostname;
  int port, status;
  char const *exchange;
  char const *exchangetype;
  char const *bindingkey;
  amqp_socket_t *socket = NULL;
  amqp_connection_state_t  conn;

  amqp_bytes_t queuename;

  hostname = "localhost";
  port = atoi("5672");
  exchange = "reply";
  exchangetype = "direct";
  bindingkey = "reply";

  conn = amqp_new_connection();

  socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    die("creating TCP socket");
  }
  status = amqp_socket_open(socket, hostname, port);
  if (status) {
    die("opening TCP socket");
  }

  {

  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest"),
                    "Logging in");
  amqp_channel_open(conn, 2);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

  }

  {
    amqp_exchange_declare(conn,2,amqp_cstring_bytes(exchange),amqp_cstring_bytes(exchangetype),
                          0,0,0,0,amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn),"Declaring exchange");
  }

  {
    amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 2, amqp_empty_bytes, 0,0,0,1,
                                                    amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn),"Declaring queue");
    queuename = amqp_bytes_malloc_dup(r->queue);
    printf(" queuename = %s\n",queuename.bytes);
    if (queuename.bytes == NULL) {
      fprintf(stderr, "Out of memory while copying queue name");
      exit(1);
    }
  }


  amqp_queue_bind(conn, 2, queuename, amqp_cstring_bytes(exchange),amqp_cstring_bytes(bindingkey),
                  amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn),"Binding queue");

  amqp_basic_consume(conn, 2, queuename, amqp_empty_bytes, 0,1,0, amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");

  printf("wait for answer\n");

  {
    for(;;){
      printf("envelope\n");
      amqp_rpc_reply_t res;
      amqp_envelope_t envelope;

      amqp_maybe_release_buffers(conn);
      res = amqp_consume_message(conn, &envelope, NULL, 0);

      printf("break\n");
      if(AMQP_RESPONSE_NORMAL != res.reply_type){
        printf("break\n");
        break;
      }

      printf("envelope.exchange = %s\n",(char *)envelope.exchange.bytes);

      printf("Delivery %u , exchange %.*s routingkey %.*s\n",
             (unsigned) envelope.delivery_tag,
             (int) envelope.exchange.len,(char *)envelope.exchange.bytes,
             (int) envelope.routing_key.len, (char *) envelope.routing_key.bytes);

      if (envelope.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) {
        printf("Content-type: %.*s\n",
               (int) envelope.message.properties.content_type.len,
               (char *) envelope.message.properties.content_type.bytes);
      }
      printf("----\n");

      amqp_dump(envelope.message.body.bytes, envelope.message.body.len);
      amqp_destroy_envelope(&envelope);
    }
  }
  die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS),"Closing channel");
  die_on_amqp_error(amqp_connection_close(conn,AMQP_REPLY_SUCCESS),"Closing connection");
  die_on_error(amqp_destroy_connection(conn), "Ending connection");

}


int main()
{
  int iret1,iret2;
  char * msg1 = "hello one";
  pthread_t thread1,thread2 ;


  iret1 = pthread_create(&thread1,NULL,send_msg_async,(void*)msg1);
  if(iret1){
    fprintf(stderr,"Error -");
    exit(EXIT_FAILURE);
  }

  iret2 = pthread_create(&thread2,NULL,receive,NULL);
  if(iret2){
    fprintf(stderr,"Error -");
    exit(EXIT_FAILURE);
  }


  pthread_join(thread1,NULL);
  pthread_join(thread2,NULL);

  exit(EXIT_SUCCESS);
}
