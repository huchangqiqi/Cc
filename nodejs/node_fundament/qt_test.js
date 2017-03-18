// setTimeout 接受函数或者字符串作为参数
// for(var i=0; i<5; i++){
// 	setTimeout(function(){
// 		console.log(i);
// 	}, 1000 * i);
// }

/*
	* 加一个闭包，使内部函数具有 i 的引用
	*/
// for(var i=0;i<5; i++){
// 	(function(i){
// 		setTimeout(function(){
// 			console.log(i);
// 		},1000 * i);
// 	})(i);
// }

/*
	* 删掉函数中的i ，内部函数没有对 i 的引用
	*/
// for(var i=0;i<5; i++){
// 	(function(){
// 		setTimeout(function(){
// 			console.log(i);
// 		},1000 * i);
// 	})(i);
// }

// 传递给setTimeout 一个IIFE ，等价于 setTimeout(undefined, ...);
//IIFE 会立即执行， 立即输出 0 1 2 3 4
for(var i=0; i<5; i++){
	setTimeout((function(i){
			console.log(i);
	})(i), 1000 * i);
}

/*
	*
	* setTimeout 里面的回调 macrotask 会在下一个Eventloop中执行
	* Promise 里面的函数是直接执行的
	* Promise.then 里面的回调 microtask 会在当前Eventloop的最后执行
	*/
// setTimeout(function(){
// 	console.log(1);
// });

// new Promise(function execotor(resolve){
// 	console.log(2);
// 	for(var i=1; i<10000; i++){
// 		i == 9999 && resolve();
// 	}
// 	console.log(3);
// }).then(function(){
// 	console.log(4);
// });

// console.log(5);
