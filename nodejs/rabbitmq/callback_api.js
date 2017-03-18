var q = 'tasks';

function bail(err){
	console.error(err);
	process.exit(1);
}

function publisher(conn) {
	conn.createChannel(on_open);
	function on_open(err, ch) {
		if(err != null) bail(err);
		ch.assertQueue(q);
		ch.sendToQueue(q, new Buffer('something to do'));
	}
}

function consumer(conn) {
	var ok = conn.createChannel(on_open);
	function on_open(err, ch) {
		if(err != null) bail(err);
		ch.assertQueue(q);
		ch.consume(q, function(msg){
			if(msg !== null){
				console.log(msg.content.toString());
			}
		});
	}
}

require('amqplib/callback_api')
	.connect('amqp://10.9.5.31', function(err, conn){
	if(err != null) bail(err);
	consumer(conn);
	publisher(conn);
});
