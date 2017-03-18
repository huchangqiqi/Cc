'use strict';
const co = require('co');
const amqp = require('amqplib');

co(function* (){
    const myConsumer = (msg) => {
        if(msg !== null){
            console.log('consuming message %s in generator', JSON.stringify(msg.content.toString()));
        }
    };

    const conn = yield amqp.connect('amqp://localhost');
    try {
        const channel = yield conn.createChannel();
        var ex = '10.0.0.1';
        yield channel.assertExchange(ex,'direct',{durable:false});
        var qok = yield channel.assertQueue('',{exclusive: true});
        var queue = qok.queue;
        yield channel.bindQueue(queue,ex,'vm');
        console.log(queue);

        //consuming the message
        yield channel.consume(queue,myConsumer,{ noAck: true });
    }
    catch(e){
        throw e;
    }
}).catch(err => {
    console.warn('Error:', err);
});

