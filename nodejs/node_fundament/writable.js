// 简单情况下不需要创建一类流， 而只是一个流对象，
'usr strict';
const Writable = require('stream').Writable;

const writable = Writable();
writable._write = function(data, enc, next){
	process.stdout.write(data.toString().toUpperCase());
	process.nextTick(next);
};

writable.on('finish', () => process.stdout.write('DONE'));

writable.write('a' + '\n');
writable.write('b' + '\n');
writable.write('c' + '\n');

writable.end();
/*
上游通过调用writable.write(data)将数据写入可写流中。write()方法会调用_write()将data写入底层。
在_write中，当数据成功写入底层后，必须调用next(err)告诉流开始处理下一个数据。
next的调用既可以是同步的，也可以是异步的。
上游必须调用writable.end(data)来结束可写流，data是可选的。此后，不能再调用write新增数据。
在end方法调用后，当所有底层的写操作均完成时，会触发finish事件。
*/
