// loading configuration files

var fs = require('fs');

//
var config = JSON.parse(fs.readFileSync('./config.json').toString());
doThisThing(config);

//
try{
	fs.readFileSync('./config.json');
} catch (err){
	console.error(err);
}
