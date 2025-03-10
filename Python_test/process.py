import os
from multiprocessing import Process,Queue
from multiprocessing import Pool
import os,time,random
'''
print(" Process %s start..." %os.getpid())
pid = os.fork()
if pid == 0:
    print('I am child Process %s,my parent is %s' %(os.getpid(),os.getppid()))
else:
    print('I am parent Process %s,my child is %s' %(os.getpid(),pid))


#multiprocessing : create process
def run_proc(name):
    print('Child Process %s %s is running...' %(name,os.getpid()))

if __name__ == '__main__':
    print('Parent Process %s is running...' %os.getpid())
    p = Process(target=run_proc,args=('testProcess',))
    p.start()
    p.join()

# process pool

def run_task(name):
    print('Run task %s' %name)
    s_start = time.time()
    time.sleep(random.random())
    s_end = time.time()
    print('Task %s run %.2f sec' %(name,s_end - s_start))
    
if __name__ == '__main__':
    print('Parent Process %s is running...' %os.getpid())
    
    p = Pool(5) # create pool contain 5 process
    for i in range(5):
        p.apply_async(run_task,args=(i,))
    print('all subProcess will running...')
    p.close() #close pool
    p.join()
    print('all subProcess running ok')
'''
def write(q):
    print('write Process %s is running...' %os.getpid())
    for x in ['python','c','java']:
        q.put(x)
        print('write to queue:%s' %x)
    q.put(None) # end signal

def read(q):
    print('read Process %s is running...' %os.getpid())
    while True:
        r = q.get(True) # block get value from Queue
        if r is None:
            print('End siganl recived, exiting...')
            break
        print('read from Queue:%s' %r)
   

if __name__ == '__main__':
    print('MainProcess %s is running...' %os.getpid())
    q = Queue()
    p1 = Process(target=write,args=(q,))
    p2 = Process(target=read,args=(q,))    
    
    p1.start()
    p2.start()
    p1.join()
    p2.join()
    
    
