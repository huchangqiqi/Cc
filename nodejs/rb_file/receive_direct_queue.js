#!/usr/bin/env node

var amqp = require('amqplib');
var routingKey = 'vm';
amqp.connect('amqp://10.9.5.31').then(function(conn) {
  process.once('SIGINT', function() { conn.close(); });
  return conn.createChannel().then(function(ch){
    var ex = '10.0.0.1';
    var ok = ch.assertExchange(ex, 'direct', {durable: false});

    ok = ok.then(function() {
      return ch.assertQueue('', {exclusive: true});
    });

    ok = ok.then(function(qok) {
        var queue = qok.queue;
        ch.bindQueue(queue, ex, routingKey);
        return queue;
    });

    ok = ok.then(function(queue) {
      return ch.consume(queue, replyMessage, {noAck: true});
    });
    return ok.then(function() {
      console.log(' [*] Waiting . To exit press CTRL+C.');
    });

    function replyMessage(msg) {
        console.log(" [x] %s:'%s',msg_id = %s",
                    msg.fields.routingKey,
                    msg.content.toString(),
		    msg.properties.messageId);
	    /*
	    ch.publish(rex,'reply',new Buffer('reply message.'),
            {correlationId: msg.properties.correlationId,
                            messageId:msg.properties.messageId});
	    */
	    
            ch.sendToQueue(msg.properties.replyTo,
                           new Buffer('reply message.'),
                           {messageId:msg.properties.messageId});

    }
  });
}).catch(console.warn);
