
var child_process = require('child_process');
var du = child_process.spawn('du', ['-sh', '/home']);

du.stdout.on('data', function(data){
	console.log('stdout:' + data);
});
du.stderr.on('data', function(data){
	console.log('stderr:' + data);
});
du.on('exit', function(code){
	console.log('child process exited with code: ' + code);
});

const exec = require('child_process').exec;
exec('cat *.js | wc -l', (error, stdout, stderr) => {
	if(error){
		console.error(`exec error: ${error}`);
		return;
	}
	console.log(`stdout: ${stdout}`);
	console.log(`stderr: ${stderr}`);
});

const execFile = require('child_process').execFile;
const child = execFile('node',['--version'], (error, stdout, stderr) => {
	if(error){
		throw error;
	}
	console.log(stdout);
});
