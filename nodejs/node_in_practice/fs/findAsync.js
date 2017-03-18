var fs = require('fs');
var join = require('path').join;

exports.findAsync = function(nameRe, startPath, cb) {
	var results = [];
	var asyncOps = 0;
	var errored = false;

	function error (err){
		if(!errored) cb(err);
		errored = true;
	}

	function finder (path) {
		asyncOps++; // increment counter before each async operation
		fs.readdir(path, function(err, files){
			if(err) return error(err);

			files.forEach(function(file){
				var fpath = join(path, file);

				asyncOps++;
				fs.stat(fpath, function(err, stats){
					if(err) return error(err);

					if(stats.isDirectory()) finder(fpath);
					if(stats.isFile() && nameRe.test(file)) results.push(fpath);

					asyncOps--;
					if(asyncOps == 0) cb(null, results);
				});
			});

			asyncOps--; //decrement counter after an async operation has completed
			if(asyncOps == 0) cb(null, results);
		});
	}
	finder(startPath);
};
