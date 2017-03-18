
function route(handle, pathname, response, request){
    console.log("About to route a request a request for " + pathname);
    if(typeof handle[pathname] === 'function'){
        handle[pathname](response, request);
    }else{
        console.log("NO request handler found for" + pathname);
        response.writeHead(404,{"Content-Type":"text/plain"});
        response.write("404 NOT FOUND");
        response.end();
    }
}
exports.route = route;
