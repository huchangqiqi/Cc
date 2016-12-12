import time
import os


def child(wpipe):
    print("hello from child", os.getpid())
    while True:
        msg = 'how are you\n'.encode()
        os.write(wpipe, msg)
        time.sleep(1)


def parent():
    rpipe, wpipe = os.pipe()
    pid = os.fork()
    if pid == 0:
        os.close(rpipe)
        child(wpipe)
        assert False, 'Fork child process error!'
    else:
        os.close(wpipe)
        print('hello form parent', os.getpid(), pid)
        fobj = os.fdopen(rpipe, 'r')
        while True:
            # recv = os.read(rpipe, 32)
            recv = fobj.readline()[:-1]
            print(recv)


if __name__ == '__main__':
    parent()
