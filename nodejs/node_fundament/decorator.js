// 类的修饰器
function testable(target) {
	target.isTestable = true;
}
@testable
class MytestableClass {}
console.log(MytestableClass.isTestable) // true

// 修饰器 本质是编译时执行的函数
@decorator
class A {}
// 等于
class A {}
A = decorator(A) || A;

// 修饰器函数的第一个参数，就是所要修饰的目标类
// 参数不够， 在修饰器外面再封装一层函数
function testable(isTestable) {
	return function(target) {
		target.isTestable = isTestable;
	};
}

// 实例属性 通过目标类的 prototype
function testable(target) {
	target.prototype.isTestable = true;
}
@testable
class MyTestableClass{}

let obj = new MyTestableClass();
obj.isTestable // true


// 方法的修饰
// class Person {
// 	@readonly
// 	name() { return `${this.first} ${this.last}`}
// }

function readonly(target, name, descriptor) {
	descriptor.writable = false;
	return descriptor;
}

readonly(Person.prototype, 'name', descriptor);
// 类似于
Object.defineProperty(Person.prototype, 'name', descriptor);


// 不能修饰函数 函数存在提升
