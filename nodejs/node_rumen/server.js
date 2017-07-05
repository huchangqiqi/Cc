var http = require("http");
var url = require("url");

function start(route, handle){
    function onRequest(request, response){
        var pathname = url.parse(request.url).pathname;
        console.log("Request for " + pathname + " received.");
        route(handle,pathname,response,request);
    }
    http.createServer(onRequest).listen(8888);
    console.log("Server has started.");
}
exports.start = start;

/*
function start(route,handle)
{
   function onRequest(request,response){
       var postData ="";
       var pathname = url.parse(request.url).pathname;
       console.log("Request for " + pathname + " received.");
       //route(handle,pathname,response);

       request.setEncoding("utf8");

       request.addListener("data", function(postDataChunk){
           postData += postDataChunk;
           console.log("received post data chunk '" + postDataChunk +"'.");
           console.log(postData);
       });

       request.addListener("end",function(){
           route(handle, pathname, response, postData);
       });
    }
    http.createServer(onRequest).listen(8888);
    console.log("server has started.");
}

exports.start = start;
*/
