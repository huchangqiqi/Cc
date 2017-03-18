var Database = require('./database');
var client = new Database('./test.db');

client.on('load', function(){
	var foo = client.get('foo');
	client.set('bar', 'my sweet value', function (err) {
		if (err) { return console.error(err); }
		console.log('write successful');
	});
	client.del('baz');
});
