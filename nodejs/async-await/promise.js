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

getRepoData()
	.then((result) => console.log(result))
	.catch((reason) => console.error(reason));
