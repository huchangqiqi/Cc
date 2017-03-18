'use strict';
const Transform = require('stream').Transform;

class Rotate extends Transform {
	constructor(n) {
		super();
		this.offset = (n || 13) % 26; //将字母旋转n 个位置
	}
	// 将可写端写入的数据变换之后添加到可读端
	_transform(buf, enc, next) {
		var res = buf.toString().split('').map(c => {
			var code = c.charCodeAt(0);
			if(c >= 'a' && c <= 'z'){
				code += this.offset;
				if(code > 'z'.charCodeAt(0)){
					code -= 26;
				}
			}else if(c >= 'A' && c <= 'Z'){
				code += this.offset;
				if(code >'Z'.charCodeAt(0)){
					code -= 26;
				}
			}
			return String.fromCharCode(code);
		}).join('');

		// 调用push 方法将变换之后的数据添加到可读端
		this.push(res);

		next();
	}
}

var transform = new Rotate(3);
transform.on('data', data => process.stdout.write(data));

transform.write('hello, ');
transform.write('world!');

transform.end();
