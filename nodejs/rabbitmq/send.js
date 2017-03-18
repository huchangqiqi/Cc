
var amqp = require('amqplib/callback_api');
amqp.connect('amqp://10.9.5.31', function(err, conn){
    conn.createChannel(function(err, ch){
        var q = 'hello';
        ch.assertQueue(q,{durable:false});
        ch.sendToQueue(q,new Buffer('hello world!!!'));
        console.log("[x] Sent 'Hello World!'");
    });
});
