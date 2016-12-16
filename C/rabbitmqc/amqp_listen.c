#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>
#include <assert.h>

#include "utils.h"



int main(int argc, char const *const *argv)
{

  char const *hostname;
  int port, status;
  char const *exchange;
  char const *bindingkey;
  char const *routingkey;
  amqp_socket_t *socket = NULL;
  amqp_connection_state_t  conn;

  amqp_bytes_t queuename;

  if(argc < 5){
    fprintf(stderr, "Usage: amqp_listen host port exchange bindingkey\n");
    return 1;
  }

  hostname = argv[1];
  port = atoi(argv[2]);
  exchange = argv[3];
  bindingkey = argv[4];

  /*
   *  conn channel
   */
  conn = amqp_new_connection();

  socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    die("creating TCP socket");
  }
  status = amqp_socket_open(socket, hostname, port);
  if (status) {
    die("opening TCP socket");
  }

  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest"),
                    "Logging in");
  amqp_channel_open(conn, 1);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

  /*
   * queue
   */

  {
    amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, amqp_empty_bytes, 0,0,0,1,
                                                    amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn),"Declaring queue");
    queuename = amqp_bytes_malloc_dup(r->queue);
    if (queuename.bytes == NULL) {
      fprintf(stderr, "Out of memory while copying queue name");
      return 1;
    }
  }

  amqp_queue_bind(conn, 1, queuename, amqp_cstring_bytes(exchange),amqp_cstring_bytes(bindingkey),
                  amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn),"Binding queue");
  amqp_basic_consume(conn, 1, queuename, amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn),"Consuming");



  {
    for(;;){
      amqp_rpc_reply_t res;
      amqp_envelope_t envelope;

      amqp_maybe_release_buffers(conn);
      res = amqp_consume_message(conn, &envelope, NULL, 0);

      if(AMQP_RESPONSE_NORMAL != res.reply_type){
        break;
      }

      amqp_basic_properties_t replyprops;
      replyprops._flags = AMQP_BASIC_CONTENT_TYPE_FLAG |
        AMQP_BASIC_DELIVERY_MODE_FLAG |
        AMQP_BASIC_CORRELATION_ID_FLAG;

      replyprops.content_type = amqp_cstring_bytes("text/plain");
      replyprops.delivery_mode = 2;
      replyprops.correlation_id = envelope.message.properties.correlation_id;
      routingkey = (char *) envelope.message.properties.reply_to.bytes;

      printf("listening routingkey = %s\n",routingkey);
      printf("replyprops.correlation_id = %s\n",replyprops.correlation_id.bytes);

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

      {

        char *response = "listening";
        printf("%s\n", response);
        //char * response = (char *)envelope.message.body.bytes;
        die_on_error(amqp_basic_publish(conn,
                                        1,
                                        amqp_cstring_bytes(exchange),
                                        amqp_cstring_bytes(routingkey),
                                        0,
                                        0,
                                        &replyprops,
                                        amqp_cstring_bytes(response)),
                     "response");


      }
      amqp_destroy_envelope(&envelope);
      printf("published\n");
    }
  }

  die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS),"Closing channel");
  die_on_amqp_error(amqp_connection_close(conn,AMQP_REPLY_SUCCESS),"Closing connection");
  die_on_error(amqp_destroy_connection(conn), "Ending connection");

  return 0;
}
