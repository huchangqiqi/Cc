
var readline = require('./node_readline');
var printer = require('./printer');

function READ(str){
	return str;
};

function EVAL(ast, env){
	return ast;
};

function PRINT(str){
	return str;
};

var rep =  function(str) { return PRINT(EVAL(READ(str),{})); };

// main-loop

while(true){
	var line = readline.readline("user> ");
	if(line === null) {break;}
	try{
		if(line) {printer.println(rep(line));}
	}catch(err){
		if(err.stack) {printer.println(err.stack);}
		else {printer.println(err);}
	}
}
