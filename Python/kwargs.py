
def func(str, **kwargs):
    for key in kwargs:
        str += key + "=" + '%d' % (kwargs[key])
    print(str)


if __name__ == '__main__':
    func("hello", quota=20, cpu_shares=200)
