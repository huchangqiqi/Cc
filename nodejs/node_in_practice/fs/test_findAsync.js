var finder = require('./findAsync');

var result = finder.findAsync(/f.*/, '.', (err,result) => {
		return console.log(result);
});
