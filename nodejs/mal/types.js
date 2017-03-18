
var types = {};

function _obj_type(obj) {
	if(_symbol_Q(obj)) { return 'symbol';}
	else if(_nil_Q(obj)) { return 'list';}
	else if(_true_Q(obj)) { return 'true';}
	else if(_false_Q(obj)) {return 'false';}
	else if(_atom_Q(obj)) { return 'atom';}
	else {
		switch(typeof(obj)) {
		case 'number' : return 'number';
		case 'string': return obj[0] == '\u029e' ? 'keyword' : 'string';
		default: throw new Error("Unknown type '" + typeof(obj) +"'");
		}
	}
}
// scalars (simple/single)
function _nil_Q(a) { return a === null ? true : false; }
function _true_Q(a) { return a === true ? true : false;}
function _false_Q(a) { return a === false ? true : false;}
function _string_Q(obj) {
	return typeof obj === 'string' && obj[0] !== '\u029e';
}

//symbol
function Symbol(name) {
	this.value = name;
	return this;
}
Symbol.prototype.toString = function () { return this.value; };
function _symbol(name) { return new Symbol(name); }
function _symbol_Q(obj) { return obj instanceof Symbol;}

// atoms
function Atom(val) { this.val = val;}
function _atom(val) {return new Atom(val);}
function _atom_Q(atm) { return atm instanceof Atom; }

exports._obj_type = types._obj_type = _obj_type;
exports._nil_Q = types._nil_Q = _nil_Q;
exports._true_Q = types._true_Q = _true_Q;
exports._false_Q = types._false_Q = _false_Q;
exports._string_Q = types._string_Q = _string_Q;
exports._symbol = types._symbol = _symbol;
exports._symbol_Q = types._symbol_Q = _symbol_Q;
exports._atom = types._atom = _atom;
exports._atom_Q = types._atom_Q = _atom_Q;
