
var restify = require('restify');

var server = restify.createServer({
	name: 'routing'
});

server.listen(8080);

function send(req, res, next){
	res.send('hello ' + req.params.name);
	return next();
}

// 响应POST请求
server.post('/hello', function create(req, res, next){
	res.send(201, Math.random().toString(36).substr(3, 8));
});
// 响应PUT请求
server.put('/hello', send);
// 响应GET请求
server.get('/hello/:name', send);
// 响应head请求
server.head('/hello/:name', send);
// 响应DEL请求
server.del('hello/:name', function rm(req, res, next){
	res.send(204);
	return next();
});

server.get(/^\/([a-zA-Z0-9_\.~-]+)\/(.*)/, function(req, res, next) {
  console.log(req.params[0]);
  console.log(req.params[1]);
  res.send(200);
  return next();
});

/*
事件监听

after 在所有handler之后执行
uncaughtException 未处理的异常
*/
