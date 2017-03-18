
var amqp = require('amqplib');
var basename = require('path').basename;
var Promise = require('bluebird');
var uuid = require('node-uuid');


amqp.connect('amqp://10.9.5.31').then(function(conn){
	return conn.createChannel().then(function(ch){
		return new Promise(function(resolve){
			var corrId = uuid();
			function maybeAnswer(msg){
				if(msg.properties.correlationId === corrId){
					resolve(msg.content.toString());
				}
			}
			var ok = ch.assertQueue('',{exclusive: true})
					.then(function(qok){return qok.queue;});

			ok = ok.then(function(queue){
				return ch.consume(queue,maybeAnswer,{noAck:true})
					.then(function(){return queue;});
			});

			ok = ok.then(function(queue){
				console.log('[X] sending msg ');
				ch.publish("exchange_for_sender",
									 "routingKey",
									 new Buffer("sender_msg"),
									 {messageId:1});
			});
		});
	});
}).catch(console.warn);
