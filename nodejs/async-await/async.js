const request = require('request');

const options = {
	url:'https://api.github.com/repos/cpselvis/zhihu-crawler',
	headers: {
		'User-Agent':'request'
	}
};

const getRepoData = () => {
	return new Promise((resolve, reject) => {
		request(options, (err, res, body) => {
			if(err){
				reject(err);
			}
			resolve(body);
		});
	});
};

async function asyncFunc() {
	try{
		const value = await getRepoData();
		// const r1 = await getR1();
		return value;
	}catch (err) {
		console.log(err);
	}
}
asyncFunc().then( x => console.log(`x: ${x}`)).catch(err => console.error(err));
