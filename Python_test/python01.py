#!/usr/bin/env python3

import math
import time
import random
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

def guess_num():
    random_num = random.randint(1,100)
    while(1):
        print('Please input a number 1~100:')
        num = int(input())
        if num > 100 or num < 1:
            print('out of range')
            continue
        if num > random_num:
            print('your number is bigger')
        elif num < random_num:
            print('your number is samller')
        else:
            print('correct')
            break
guess_num()


if __name__ == "__main__":
    for i in range(1,10):
        for j in range (1,i+1):
            print(f'{j}x{i}={i*j}',end=" ")

        print()

# fibonacci num
def fibo_num(n):
    lst = [0,1]
    for i in range(n-2):
        lst.append(lst[-1]+lst[-2])
    return lst
def fibo_num2(n):
    lst = [0 ,1]
    for i in range(2,n):
        lst.append(lst[i-2]+lst[i-1])
    return lst
def fibo_num1(n):
    if n == 1:
        return 0
    if n == 2:
        return 1
    n1,n2 = 0,1
    for i in range(2,n):
        n3 = n1+n2
        n1 = n2
        n2 = n3
    return n3

def is_prime(n):
    if n<=1:
        return False
    for i in range(2,int(math.sqrt(n))+1):
        if n % i == 0:
            return False
    return True

def choice(*args):
    return [ i for i in args if is_prime(i)]
if __name__ =="__main__":
    #result = fibo_num(10)
    # result = fibo_num1(10)
    prime_number=choice(1,3,5,7,9,11,13,15,17,19)
    print (prime_number)
    

import sys

def test():
    args = sys.argv
    if len(args) == 1:
        print("hello,world!")
    elif len(args) == 2:
        print(f'hello,{args[1]}!')
    else:
        print("too many arguments!")
    print(__name__)

if __name__ == '__main__':
    test()
    
import hello

result = hello.greet('lee')
print(result)


class Student:
    def __init__(self,name,score,gender):
        self.name = name
        self.score = score
        self.__gender = gender
    
    def get_score(self):
        print(f'{self.name}:{self.score}')

    def get_gender(self):
        return self.__gender
    
    def set_gender(self,gender1):
        self.__gender =gender1

    def get_grade(self):
        if self.score >= 90:
            return 'A'
        elif self.score >=60:
            return 'B'
        else:
            return 'C'
        
    
bart = Student('Bart Simpson',78,'male')
if bart.get_gender() != 'male':
    print('failed')
else:
    bart.set_gender('female')
    if bart.get_gender() != 'female':
        print('failed')
    else:
        print('success')

class Animal:
    def run(self):
        print('Animal is running...')

class Dog(Animal):
    def run(self):
        print('Dog is running...')
    def eat(self):
        print('Eating meat...')

class Cat(Animal):
    def run(self):
        print('Cat is running...')
    def eat(self):
        print('Eating fish...')
class Tortoise:
    def run(self):
        print('Tortoise is running slowly...')
dog = Dog()
cat = Cat()
def run_twice(animal):
    animal.run()

run_twice(Animal())
run_twice(Dog())
run_twice(Cat())
run_twice(Tortoise())



class Student():
    count = 0
    def __init__(self,name):
        self.name = name
        #self.increment_count()
        Student.count += 1
    
    @classmethod
    def increment_count(cls):
        cls.count +=1
        


if Student.count !=0:
    print('failed!')
else:
    bart = Student('Bart')
    if Student.count != 1:
        print('failed2')
    else:
        lisa = Student('lisa')
        if Student.count != 2:
            print('failed')
        else:
            print(f'students:{Student.count}') 
            print('success')  
print(lisa.count)

"""

class Screen:
    @property
    def width(self):
        return self._width
    
    @width.setter 
    def width(self,value):
        if not isinstance(value,int): 
            raise ValueError('value must be an interger!')  
        else:
            self._width = value
    
    @property
    def height(self):
        return self._height
    @width.setter
    def height(self,value):
        if not isinstance(value,int):
            raise ValueError('value must be an interger!')
        else:
            self._height = value

    @property
    def resolution(self):
        return self._height*self._width
s = Screen()
s.width = 1024
s.height = 768
print(f'resolution = {s.resolution}')
if s.resolution == 786432: 
    print('success')
else:
    print('failed')