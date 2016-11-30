import time

class Timeit(object):
    def __init__(self,func):
        self.func = func
    def __call__(self,*args,**kwargs):
        print('invoking Timer')
    def __get__(self,instance,owner):
        return lambda *args,**kwargs:self.func(instance,*args,**kwargs)

#
#    def __call__(self,*args,**kws):
#        start_time = time.time()
#        result = self._wrapped(*args,**kws)
#        print("elapsed time is %s" %(time.time() - start_time))
#        return result



#
#@Timeit
#def func():
#    time.sleep(1)
#    return "invoking function func"

class A(object):
    @Timeit
    def func(self):
        time.sleep(1)
        return 'invoking methond func'



if __name__ == '__main__':
    a = A()
    a.func()
