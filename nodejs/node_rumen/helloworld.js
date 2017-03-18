console.log("hello World");

function say(word) {
    console.log(word);
}

function execute(someFunction,value) {
    someFunction(value);
}

execute(say,"spike");
execute(function(word){console.log(word);},"faye");
