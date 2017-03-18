var q = 'tasks';

var open = require('amqplib').connect('amqp://10.9.5.31');

//publisher
open.then(function(conn){
	return conn.createChannel();
}).then(function(ch){
	return ch.assertQueue(q).then(function(ok){
		return ch.sendToQueue(q, new Buffer('something to do'));
	});
}).catch(console.warn);

//Consumer
open.then(function(conn){
	return conn.createChannel();
}).then(function(ch){
	return ch.assertQueue(q).then(function(ok){
		return ch.consume(q, function(msg) {
			if(msg !== null){
				console.log(msg.content.toString());
			}
		});
	});
}).catch(console.warn);
