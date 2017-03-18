#include <node/node.h>
#include <node/v8.h>

using namespace v8;

void MyFunction(const v8::FunctionCallbackInfo<Value>& args){
    Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Hello world!" ));
}

void Test(const v8::FunctionCallbackInfo<v8::Value> args){
    Isolate* isolate = args.GetIsolate();
    //Number
    Local<Number> retval = v8::Number::New(isolate,1000);
}

void init(Local <Object> exports, Local<Object> module) {
    NODE_SET_METHOD(exports, "getTestValue", Test);
}

NODE_MODULE(returnValue, init)

