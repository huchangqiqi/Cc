var EventEmitter = require('events').EventEmitter;
var util = require('util');

function MyEmitter(){
	EventEmitter.call(this);
}
util.inherits(MyEmitter, EventEmitter);

MyEmitter.prototype.doStuff = function doStuff(){
	var self = this;
	console.log('before');
	self.emit('fire');
	console.log('after');
};

var me = new MyEmitter();
me.on('fire', function(){
	console.log('emit fired');
});
me.doStuff();
