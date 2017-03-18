// createChannel 无法运行  createConfirmChannel 可以运行
'use strict';
const co = require('co');
const amqp = require('amqplib');

co(function* (){
    const conn = yield amqp.connect('amqp://localhost');
    try{
        var routingkey = 'vm';
        var ex = 'direct_logs';
        var msg = 'Hello world';
        const channel = yield conn.createConfirmChannel();
        yield channel.assertExchange(ex, 'direct', {durable: false});
        channel.publish(ex, routingkey ,new Buffer(msg));
        yield channel.waitForConfirms();
        console.log("[x] Sent %s : '%s'",routingkey, msg);
        channel.close();
    }
    catch(e){
        throw e;
    }
    finally{
        conn.close();
    }
}).catch(err => {
    console.warn('Error:', err);
});

