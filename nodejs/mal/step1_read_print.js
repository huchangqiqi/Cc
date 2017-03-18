var types = require('./types');
var readline = require('./node_readline');
var printer = require('./printer');
var reader = require('./reader');

function READ(str){
	return reader.read_str(str);
}

function EVAL(ast, env) {
	return ast;
}

function PRINT(exp) {
	return printer._pr_str(exp, true);
}

var rep = function(str){ return PRINT(EVAL(READ(str), {}));};

while(true) {
	var line = readline.readline("user> ");
	if(line === null) {break;}
	try{
		if(line) {printer.println(rep(line));}
	}catch(exc){
		if (exc instanceof reader.BlankException) { continue;}
    if (exc.stack) { printer.println(exc.stack); }
    else           { printer.println(exc); }
	}
}
