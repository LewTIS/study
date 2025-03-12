# coding: utf-8
# 包含yield关键字的函数都是生成器函数，调用生成器函数时，函数内部的代码并不会立即执行，而是返回一个生成器对象。
def consumer():
    r =''
    while True:
        n = yield r # yield r 会返回r的值，并且暂停执行，等待下一次调用
        print('[consumer] consuming %s' % n)
        r = '200 ok'
        
def produce(c):
    c.send(None) # 启动生成器，第一次调用时参数必须为None, 开始执行生成器函数到第一个yield
    i = 0
    while i < 5:
        i = i + 1
        print('[produce] Start produce %s' % i)
        r = c.send(i) # 传入i, 生成器函数继续执行，直到下一个yield
        print('[produce] consumer return %s' % r)
        
c = consumer() # create a generator
produce(c)  
        