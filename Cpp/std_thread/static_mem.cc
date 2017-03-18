#include <iostream>
#include <thread>
#include <unistd.h>

class Test
{
public:
    Test(int m):n(m),testThread_(print,this){

    }
    static void print(Test *pt){
        std::cout << pt->n << std::endl;
    }
private:
    int n;
    std::thread testThread_;
};

int main(int argc,char *argv[]){
    Test test(8);
    sleep(1000);
    return 0;
}
