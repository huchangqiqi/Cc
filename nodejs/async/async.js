var async = require('async');

var objs = [{name:'a'},{name:'b'},{name:'c'}];

function doSomething(obj, cb){
	console.log("doing " + obj.name);
	cb(null,obj);
}

async.each(objs, function(obj, callback){
	doSomething(obj, function(){
		callback();
	});
}, function(err){
	console.log("err is" + err);
});

// eachSeries forEachSeries
// same as each but runs only a single async operation at time

//waterfall

async.waterfall([
	function(callback){
		callback(null,'one','two');
	},
	function(arg1, arg2, callback){
		// arg1 now equals 'one' and arg2 now equals 'two'
		callback(null, 'three');
	},
	function(arg1, callback){
		// arg1 now equals 'three'
		callback(null, 'done');
	}
], function(err, result){
// result now equals 'done'
});

//whilst

var count = 0;
async.whilst(
	// 条件
	function(){return count < 5;},
	//循环主体
	function(callback){
		count++;
		setTimeout(function(){
			callback(null, count);
		}, 1000);
	},
	// 条件不满足 或者发生异常
	function(err, n){
		// 5 seconds have passed, n = 5
	}
);
