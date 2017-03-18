
var amqp = require('amqplib/callback_api');
amqp.connect('amqp://10.9.5.31', function(err, conn){
    conn.createChannel(function(err, ch){
        var q = 'hello';
        ch.assertQueue(q,{durable:false});
        console.log("[*] waiting for messages in %s. To exit press CTRL+c", q);
        ch.consume(q, function(msg){
            console.log('[x] Received %s', msg.content.toString());
        },{noAck: true});
    });
});
