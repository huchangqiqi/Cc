const co = require('co');
const request = require('request');

const options = {
	url:'https://api.github.com/repos/cpselvis/zhihu-crawler',
	headers: {
		'User-Agent':'request'
	}
};

// yield 后面是一个生成器 generator
const getRepoData = function* () {
	return new Promise((resolve, reject) => {
		request(options, (err, res, body) => {
			if(err){
				reject(err);
			}
			resolve(body);
		});
	});
};

co(function* (){
	const result = yield getRepoData;
	//... 如果多个异步流程，可以放在这里， 每个yield相当于暂停， 执行yield之后会等待它后面的generator返回值之后再执行后面其它yield逻辑
	// const r1 = yield getR1;
	// const r2 = yield getR2;

	return result;
}).then(function (value){
	console.log(value);
}, function(err) {
	console.error(err.stack);
});
