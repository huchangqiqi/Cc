#!/usr/bin/env node

var amqp = require('amqplib');

var routingKey = 'vm';
var message = 'Hello World!';

amqp.connect('amqp://localhost').then(function(conn) {
  return conn.createChannel().then(function(ch) {
    var ex = '10.0.0.1';
    var ok = ch.assertExchange(ex, 'direct', {durable: false});

    return ok.then(function() {
      ch.publish(ex, routingKey , new Buffer(message));
      console.log(" [x] Sent %s:'%s'", routingKey, message);
      return ch.close();
    });
  }).finally(function() { conn.close(); });
}).catch(console.warn);
