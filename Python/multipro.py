
from multiprocessing import Process, Queue, Pipe, Lock


def f(conn):
    conn.send([42, None, 'hello'])
    conn.close()


"""
if __name__ == '__main__':
    parent_conn, child_conn = Pipe()
    p = Process(target=f, args=(child_conn,))
    p.start()
    print(parent_conn.recv())
    p.join()

if __name__ == '__main__':
    q = Queue()
    p = Process(target=f, args=(q,))
    p.strat()
    print(q.get())
    p.join()
"""


def fl(l, i):
    l.acquire()
    try:
        print('hello world', i)
    finally:
        l.release()


if __name__ == '__main__':
    lock = Lock()
    for num in range(10):
        Process(target=fl, args=(lock, num)).start()
