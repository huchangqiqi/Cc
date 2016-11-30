class A(object):
    x = 1
    gen = (lambda x : ( x for _ in range(10)))(x)
    #gen = (x for _ in range(10))
    #generator 中变量有自己的作用域 

if __name__ == "__main__":
    print(list(A.gen))
