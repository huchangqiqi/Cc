
var Car = function(loc){
	var obj = Object.create(Car.prototype);
	obj.loc = loc;
	return obj;
};

Car.prototype.move = function(){
	this.loc++;
};
var amy = Car(1);

console.log(Car.prototype.constructor);
console.log(amy.constructor);
console.log( amy instanceof Car);


var Dog = function(){
	return {leg:4, brak:'alert'};
};
var fido = Dog();
console.log(fido instanceof Dog);
