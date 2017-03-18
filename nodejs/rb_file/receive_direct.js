#!/usr/bin/env node

var amqp = require('amqplib');
var routingKey = '10.0.0.1:1001';
amqp.connect('amqp://10.9.5.31').then(function(conn) {
  process.once('SIGINT', function() { conn.close(); });
  return conn.createChannel().then(function(ch){
   	var exchange_for_sender = 'exchange_for_sender';
	var exchange_for_receiver = 'exchange_for_receiver';
    	var ok = ch.assertExchange(exchange_for_sender, 'direct', {durable: false});
	ok = ok.then(function (){
	return ch.assertExchange(exchange_for_receiver,'direct',{durable: false});
	});

    ok = ok.then(function() {
      return ch.assertQueue('', {exclusive: true});
    });

    ok = ok.then(function(qok) {
        var queue = qok.queue;
        ch.bindQueue(queue, exchange_for_sender, routingKey);
        return queue;
    });

    ok = ok.then(function(queue) {
      return ch.consume(queue, replyMessage, {noAck: true});
    });
    return ok.then(function() {
      console.log(' [*] Waiting . To exit press CTRL+C.');
    });

    function replyMessage(msg) {
        console.log(" [x] routingkey = %s:' str = %s' appId = %s",
		msg.fields.routingKey,
                msg.content.toString(),
		msg.properties.appId);

	    	ch.publish(exchange_for_receiver,
			   msg.properties.replyTo,
			   new Buffer('reply message.'),
                	   { messageId:msg.properties.messageId});

	    /*
            ch.sendToQueue(msg.properties.replyTo,
                           new Buffer('reply message.'),
                           {messageId:msg.properties.messageId});

	    */
	    /*
        function reply(m){
            ch.sendToQueue(msg.properties.replyTo,
                           new Buffer('reply message.'),
                           {correlationId: msg.properties.correlationId});
        }
	    */
    }
  });
}).catch(console.warn);
