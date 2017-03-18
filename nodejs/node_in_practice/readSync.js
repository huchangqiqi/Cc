var fs = require('fs');
var buff = Buffer.alloc(5);


fs.readSync(1, buff);
//fs.writeSync(1, buff.toString());
//console.log(buff);
