// object.create
var util = require('util');

function Psrson(firstname){
	this.firstname = firstname;
}

Shape.prototype.sayHello = function(){
	console.log('I am working!');
};

function Student(firstname, subject){
	Person.call(this, firstname);//call super constructor
	//初始化子类特有属性
	this.subject = subject;
}

// Object.create(proto,[propertiesObject]) proto 一个对象，作为新对象的原型，或者为null
/* 常见的错误是使用"new Person()" 来创建Student.prototype 这样错误很多， 最重要一点是实例化时不能赋予Person
	 类任何的Firstname参数

Student.prototype = Object.create(Person.prototype);
Student.prototype.constructor = Student; // 设置constructor 属性指向Student 如果不这样 Student.prototype.constructor 会指向 Person
*/
util.inherits(Student, Psrson);
Student.prototype.sayHello = function(){
	console.log("hello ,I'm " + this.firstname + "I'm studying" + this.subject);
};
var rect = new Rectangle();
console.log(rect instanceof Rectangle);
console.log(rect instanceof Shape);
rect.move(1,1);
