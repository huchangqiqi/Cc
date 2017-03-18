var reader = {};
var types = require('./types');

function Reader(tokens){
	this.tokens = tokens.map(a => {return a;});
	this.position = 0;
}

Reader.prototype.next = function(){ return this.tokens[this.position++];};
Reader.prototype.peek = function(){ return this.tokens[this.position];};

function tokenize(str) {
    var re = /[\s,]*(~@|[\[\]{}()'`~^@]|"(?:\\.|[^\\"])*"|;.*|[^\s\[\]{}('"`,;)]*)/g;
	var results = [];
	while((match = re.exec(str)[1]) != '') {
		if( match[0] === ';') {continue;}
		results.push(match);
	}
	return results;
}

function read_str(str) {
	var tokens = tokenize(str);
	if(tokens.length === 0) {
		throw new BlankException();
	}
	return read_form(new Reader(tokens));
}

function read_form(reader) {
	var token = reader.peek();
	switch(token) {
	case ';': return null; // ignore comments
	case '\'': render.next();
		return [types._symbol('quote'), read_form(reader)];
	case '`': reader.next();
		return [types._symbol('quasiquote'), read_form(reader)];

		//list
	case ')': throw new Error("unexpected ')'");
	case '(': return read_list(reader);

		//atom
	default: return read_atom(reader);
	}
}

function BlankException(msg) {
}

function read_list(reader, start, end){
	start  =  start || '(';
	end = end || ')';
	var ast = [];
	var token = reader.next();
	if(token !== start){
		throw new Error("expected '" + start + "'");
	}
	while((token = reader.peek()) != end){
		if(!token){
			throw new Error("expected '" + end + "', got EOF");
		}
		ast.push(read_form(reader));
	}
	reader.next();
	return ast;
}

function read_atom(reader) {
	var token = reader.next();
	if(token.match(/^-?[0-9]+$/)) {
		return parseInt(token,10);
	} else if(token.match(/^-?[0-9][0-9.]*$/)) {
		return parseFloat(token, 10);
	} else if(token[0] === "\"") {
		return token.slice(1, token.length-1)
			.replace(/\\"/g,'"')
			.replace(/\\n/g,"\n")
			.replace(/\\\\/g,"\\");
	} else if(token[0] === ":") {
		return types._keyword(token.slice(1));
	} else if(token === "nil") {
		return null;
	} else if(token === "true") {
		return true;
	} else if(token === "false") {
		return false;
	} else {
		return types._symbol(token);
	}
}

exports.Reader = reader.Reader = Reader;
exports.BlankException = reader.BlankException = BlankException;
exports.tokenize = reader.tokenize = tokenize;
exports.read_form = reader.read_form = read_form;
exports.read_str = reader.read_str = read_str;
