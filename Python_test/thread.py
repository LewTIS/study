import threading,time
'''
def test():
    print('Thread %s is running...' %threading.current_thread().name)
    print('waitting 3 seconds...')
    time.sleep(3)
    print('hello thread!')
    print('thread %s is end.' %threading.current_thread().name)
    
print('Thread %s is running...' %threading.current_thread().name)
t = threading.Thread(target = test,name = 'TestThread')
t.start()
t.join()
print('Thread %s is end.' %threading.current_thread().name)

'''
lock = threading.Lock()

amount = 0
def changeValue(x):
    global amount
    amount = amount + x
    amount = amount - x

def batchRunThread(x):
    for i in range(100000):
        lock.acquire()
        try:    
            changeValue(x)
        finally:
            lock.release()


        
t1 = threading.Thread(target=batchRunThread, args=(5,), name = 'Thread1')
t2 = threading.Thread(target=batchRunThread, args=(15,), name = 'Thread2')

t1.start()
t2.start()
t1.join()
t2.join()
print(amount)