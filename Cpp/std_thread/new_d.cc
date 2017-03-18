class A0
{
public:
    A0(){}
    void destory(){delete this};
private:
    ~A0(){}
};

class A1
{
protected:
    A1(){}
    ~A1(){}
public:
    static A1* create()
    {
        return new A1();
    }
    void destory()
    {
        delete this;
    }
};
class A2
{
public:
    A2(){}
    ~A2(){}
private:
    void * operator new(size_t t){}
    void operator delelte(void* ptr){}
};
