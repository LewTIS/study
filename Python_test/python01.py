#!/usr/bin/env python3

import math
import time
"""
print("hello,world")
x = input("please input x:")
y = input("please input x:")
s = int(y)
#s=len(x)

#print("The length of %s is %d" %(x,s))
print("The lengrh of {} is {}" .format(x,s))
a = "hello"
b = "world"

print(a+b)  
"""

"""
age = input("please input your age:")
age = int(age)
if age < 60:
    print("not retire")
    delta = 60 - age
    print(f"after {delta} years,you will retire")
centenarian = 100 - age
print(f"after {centenarian} years,you will be a centenarian")

sum = 0
for i in range(1,101):
    sum += i
print(f'the sum of 1 to 100 is {sum}')

word = input('please input an english world:')
result = {}
for e in word:
    if e in result:
        result[e] += 1
    else:
        result[e] = 1
print(f'the word is : {word}')
print(f'count letter:{result}')


d = { 'website':'www.baidu.com','name':'python'}
for k in d:
    print(f'key:{k} ---> value:{d[k]}')
for k,v in d.items():
    print(f'key:{k} ---> value:{v}')

a = range(1,6)
b = range(9,4,-1)
c = []
for x,y in zip(a,b):
    c.append(x*y)
print(c)


myinfor = {"publish":"phei","site":"itdiffer.com","lang":"python"}
infor = dict()
for k,v in myinfor.items():
    infor[v] = k
infor2 = dict(zip(myinfor.values(),myinfor.keys()))
print(infor2)
"""

start = time.time()
lst = [i*i for i in range(100000)]
end = time.time()
delta = end - start

print(f'list comprehension time: {delta}')

start2 = time.time()
lst2 = []
for i in range(100000):
    lst.append(i*i)
end2 = time.time()
delta2 = end2 - start2

print(f'for loop time: {delta2}')
print(f'(list comprehension) / (for loop) = {delta/delta2:.3f}')