#include <thread>
#include <unistd.h>
#include <iostream>
#include <functional>

class MyClass{
public:
    MyClass(int n):m(n)
    {
        std::thread t(std::mem_fn(&MyClass::thread_func),this);
        t.join();
    }
    void func(){
        //auto tf = std::mem_fn(&MyClass::thread_func);
    }
    void thread_func(){
        sleep(10);
        std::cout << this->m <<std::endl;
    }
    std::thread t;
private:
    int m;
};

class Test{
public:
    Test():testThread_(thread_func)
    {
        //extern std::thread testThread_(&Test::thread_func);
        testThread_.join();//testThread_.join()
    }
    static void thread_func(void){
        std::cout << "yyyy" << std::endl;
    }
private:
    std::thread testThread_;
};

int main(int argc,char *argv[]){
    MyClass myclass(2);
    //myclass.func();

    //Test test;


    //auto tf = std::mem_fn(&MyClass::thread_func);
    //tf(myclass);

    //std::thread t(tf,myclass);
    //t.join();
    return 0;
}
