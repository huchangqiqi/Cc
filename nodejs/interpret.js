
function interpret(form, env, ctx){
	if(form instanceof Array){
		switch(form[0]){
		case 'lambda':{
			var params = form[1];
			var body = form[2];
			return ctx(function(ctx){ return function(){
				var e = Object.create(env);
				for(var j=0; j < params.length; j++){
					e[params[j]] = arguments[j];
				}
				return interpret(body, e, ctx);
			};});
		};
		case 'if':{
			var test = form[1], consequent = form[2], alternate = form[3];
			return interpret(form[1], env, function(c){
				if(c) return interpret(consequent, env, ctx);
				else return interpret(alternate, env, ctx);
			});
		};
		case 'callcc':{
			return interpret(form[1], env, function(f){
				var fctx = function(){ return function(x){ return ctx(x);};};
				return f(fctx)(fctx);
			});
		};
		case 'quote':{
			return ctx(form[1]);
		};
		default: {
			return interpretCall(form, env, ctx);
		};
		}
	} else if( typeof form === 'string') {
		return ctx(env[form]);
	} else {
		return ctx(form);
	}
}

function interpretCall(form, env, ctx){
	return interpret(form[0], env, function(callee){
		return interpret$(form.slice[1], env, function(args){
			return callee(ctx).apply(null, args);
		});
	});
}

function interpret$(form, env, ctx){
	if(!form.length) return ctx(null);
	else return interpret(form[0], env, function(x0){
		return interpret$(form.slice(1), env, function(x$){
			if(x$) return ctx([x0]);
			else return ctx([x0].concat(x$));
		});
	});
}

function id(x) {return x;}

var env0 = { trace: function(ctx){ return function(x) {return ctx(console.log(x));};}};

interpret(['trace',['callcc',['lambda',['return'],['return', ['quote',3]]]]],env0, id);
