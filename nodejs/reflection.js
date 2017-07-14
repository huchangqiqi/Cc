var util = require('util');
var events = require('events');

function EventTracker(){
	events.EventEmitter.call(this);
}

util.inherits(EventTracker, events.EventEmitter);

var eventTracker = new EventTracker();

eventTracker.on('newListener', function(name, listener) {
	console.log('Event name added:', name);
});

eventTracker.on('a listener', function() {
	//this will cause 'newListener' to fire
});
