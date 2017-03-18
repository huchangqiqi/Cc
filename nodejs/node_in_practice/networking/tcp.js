
/*
  * net.createServer server.listen net.connect
	* socket.setNoDelay
	*/

var net = require('net');
var clients = 0;


/**
 * return an obj that can be used to listen on a given tcp port incomming connections
 * when a client makes a new connection, then cb passed to net.createServer will run , the cb receives a connection obj which extends EventEmitter
 * @param {} client
 */
var server = net.createServer(function(client){
	var clientId = clients++;
	console.log(` Client Connected :${clientId}`);

	client.on('end', function(){
		console.log(`Client disConnected: ${clientId}`);
	});

	client.write(`Welcome client ${clientId} rn `);
	client.pipe(client);
});

server.listen(8000, function(){
	console.log('Server start on 8000');
});
