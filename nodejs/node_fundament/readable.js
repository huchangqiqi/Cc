// 创建可读流
'use strict';
const Readable = require('stream').Readable;

class ToReabable extends Readable {
	constructor(itreable){
		super();
		this.iterator = new function *() {
			yield * itreable;
		};
	}
	// 子类需要实现该方法，这是生产数据的逻辑
	_read(){
		const res = this.iterator.next();
		if(res.done){
			this.push(null);
		}else{
			this.push(res.value + '\n');
		}
	}
}
module.exports = ToReabable;

// 实际使用时
const iterable = function *(limit){
	while(limit--){
		yield Math.random();
	}
}(1e10);
const readable = new ToReabable(iterable);
readable.on('data', data => process.stdout.write(data)); // 监听'data' 事件， 一次获取一个数据
readable.on('end',() => process.stdout.write('DONE'));   // 所有数据均已读完
// 创建可读流时， 需要继承Readable， 实现_reab 方法
// 在_read 方法中，通过调用push(data) 将数据放入可读流中供下游消耗


// use object.create
function Mystream(options) {
	Readable.call(this, options);
}
Mystream.prototype = object.create(Readable.prototype, {
	constructor: {value: Mystream}
});
Mystream.prototype._read = function(size){
};
