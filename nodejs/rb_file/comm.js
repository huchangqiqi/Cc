/**
 * Created by Gavin on 2017/1/7.
 */
var amqp = require('amqplib');
var config = require('./config.json');

//module.exports = function (module_name, log, callback) {

    var module_name = 'vm';
    var rabbitmq = config.rabbitmq;
    var rabbitmq_server_addr = 'amqp://' + rabbitmq.rabbitmq_server_addr;
    var routingKey = module_name + config.localhost + ":" + config[module_name].port ;
    amqp.connect(rabbitmq_server_addr).then(function (conn) {
        process.once('SIGINT', function () {
            conn.close();
        });
        return conn.createChannel().then(function (ch) {
            var exchange_for_sender = rabbitmq.exchange_for_sender;
            var exchange_for_receiver = rabbitmq.exchange_for_receiver;
            var ok = ch.assertExchange(exchange_for_sender, 'direct', {durable: false});
            ok = ok.then(function () {
                ch.assertExchange(exchange_for_receiver, 'direct', {durable: false});
            });

            ok = ok.then(function () {
                return ch.assertQueue('', {exclusive: true});
            });

            ok = ok.then(function (qok) {
                var queue = qok.queue;
                ch.bindQueue(queue, exchange_for_sender, routingKey);
                return queue;
            });

            ok = ok.then(function (queue) {
                return ch.consume(queue, replyMessage, {noAck: true});
            });
            return ok.then(function () {
                console.log(' [*] Waiting . To exit press CTRL+C.');
            });

            function replyMessage(msg) {
                console.log("[X] %s",msg.content.toString());
                //callback(JSON.parse(msg.content.toString()), publish);
                //function publish(reply_msg) {
                    ch.publish(exchange_for_receiver,
                               msg.properties.replyTo,
                               new Buffer('reply_msg'),
                               {messageId: msg.properties.messageId});
                //}
            }
        });
    }).catch(console.warn);

//};
