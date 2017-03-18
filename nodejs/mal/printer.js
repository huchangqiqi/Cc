
var printer = {};
var types = require('./types');

printer.println = exports.println = function(){
	console.log.apply(console, arguments);
};

function _pr_str(obj, print_readably) {
	if(typeof print_readably === 'undefined') { print_readably = true; }
	var _r = print_readably;
	var ot = types._obj_type(obj);

	switch(ot) {
	case 'list':
		var ret = obj.map((e) => {return _pr_str(e, _r);});
		return `(${ret.join(' ')})`;
	case 'nil':
		return "nil";
	case 'atom':
		return `(atom ${_pr_str(obj.val, _r)})`;
	default:
		// symbol return the string name
		// number return number as string
		return obj.toString();
	}
}

exports._pr_str = printer._pr_str = _pr_str;
