#!/usr/bin/env python3

import math
import time
import random
import datetime
import sys
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




class Book:
    prices = {"A":45.7,"B":56.7,"C":67.8,"D":78.9,"E":90.1}
    shipping = 5
    def __init__(self,book_name,book_num,free_ship):
            self.book_name = book_name
            self.num = book_num
            self.free_ship = free_ship
    
    def totals(self):
            price = Book.prices.get(self.book_name)
            
            t = price * self.num
            
            #if t < self.free_ship:
            #        return t + Book.shipping
            #else:
            #        return t
            
            return (t + Book.shipping) if t < self.free_ship else t

if __name__ == '__main__':
    book_a = Book('B',2,100)
    a_total=book_a.totals()
    print(a_total)           


class Person:
    def __init__(self,name,age):
        self.name = name
        self.age = age
    
    @classmethod
    def by_birth(cls,name,birth_year):
        this_year = datetime.date.today().year
        age = this_year - birth_year
        return cls(name,age)
    def get_info(self):
        return "{0}'s age is {1}".format(self.name,self.age)

if __name__ == "__main__":
    newton = Person('Newton',26)
    print(newton.get_info())
    hertz = Person.by_birth("Hertz",1857)
    print(hertz.get_info())


class Person:
    def __init__(self,name,age):
        self.name = name
        self.age =  age
    
    def get_name(self):
        return self.name
    
    def get_age(self):
        return self.age

class Student(Person):
    def __init__(self,school,name,age):
        self.school = school
        super().__init__(name,age)
    def grade(self,n):
        print(f"{self.name}'s grade is {n}")

if __name__ == "__main__":
    #stu1 = Student("Galiao",27)
    stu1 = Student("Social University","Galiao","24")
    stu1.grade(99)
    print(stu1.get_name())
    print(stu1.get_age())

class Cat:
    def speak(self):
        print("meow")

class Dog:
    def speak(self):
        print("woof")

class Bob:
    def bow(self):
        print("thank you")
    def speak(self):
        print("hello,welcome")
    def drive(self):
        print("beep")

def command(pet):
    pet.speak()
pets = [ Cat(),Dog(),Bob()]

for pet in pets:
    command(pet)



class Book:
    def __init__(self,name):
        self.name = name
        self.book = "mathematics for ML"
    
    def __getattr__(self,attr):
        print("not exist!")
    #def __setattr__(self,attr,value):
        #super().__setattr__(attr,value)
    def __setattr__(self,attr,value):
        if attr == 'author':
            super().__setattr__(attr,'laoqi')
        else:
            super().__setattr__(attr,value)
    def __delattr__(self,attr):
        if attr == 'book':
            raise AttributeError("can't delete")
        else:
            super().__delattr__(attr)
    
        


class Mylist(list):
    def __getitem__(self,index):
        if index == 0:
            raise IndexError
        if index > 0 :
            index = index - 1
            return list.__getitem__(self,index)
    
    def __setitem__(self,index,value):
        if index == 0:
            raise IndexError
        if index > 0:
            index = index - 1
            list.__setitem__(self,index,value)

if __name__ == "__main__":
    lst = Mylist(['python','java','C++'])
    print(lst)
    lst.append('PHP')
    print(f"lst[1] is {lst[1]}")
    print(f"lst[4] is {lst[4]}")
    lst[2] = 'R'
    print(lst)
    


class Simdict:
    def __init__(self,k,v):
        self.__dct = dict([(k,v),])
    
    def __setitem__(self,k,v):
        self.__dct[k] = v
    
    def __getitem__(self,k):
        return self.__dct[k]
    
    def __delitem__(self,k):
        del self.__dct[k]

    def __str__(self):
        return f"{self.__dct}"
    
    __repr__ = __str__

    def __len__(self):
        return len(self.__dct)

if __name__ == "__main__":
    d = Simdict('name','laoqi')
    print(d)
    d['lang'] = 'python'
    d['city'] = 'Siichow'
    print(d['city'])
    print(len(d))
    print(d)
    del d['city']
    print(d)


class Singleton:
    _instance = None
    def __new__(cls,*args,**kwargs):
        if not cls._instance:
            cls._instance = super().__new__(cls)
        return cls._instance

class MyBook(Singleton):
    def __init__(self,name):
        self.name = name

if __name__ == "__main__":
    b1 = MyBook("Python")
    b2 = MyBook("math")
    print(b1)
    print(b2)
    print("b1 is b2: ", b1 is b2)
    print(f"b1.name = {b1.name}")
    print(f"b2.name = {b2.name}")
    print("b1.name is b2.name:", b1.name is b2.name)

class MyRange:
    def __init__(self,n):
        self.i = 1
        self.n = n

    def __iter__(self):
        return self
    
    def __next__(self):
        if self.i <= self.n:
            i = self.i
            self.i += 1
            return i
        else:
            raise StopIteration()
class Fibs:
    def __init__(self,max):
        self.max = max
        self.a = 0
        self.b =1
    
    def __inter__(self):
        return self
    
    def __next__(self):
        fib = self.a
        if fib > self.max:
            raise StopIteration
        self.a , self.b = self.b,self.a + self.b
        return fib

if __name__ == "__main__":
    fibs = Fibs(100000)
    
    lst = [fibs.__next__() for i in range(26)]
    print(lst)
    

class Calculator:
    is_raise = True
    def calc(self,expression):
        try:
            return eval(expression)
        except ZeroDivisionError as e:
            print(e)
            return "expression error"
        except NameError as e:
            print(e)
            return "not exist"

if __name__ == "__main__":
    cal = Calculator()
    result = cal.calc("7/0")
    print(result)


class MyCustomError(Exception):
    def __init__(self,*args):
        if args:
            self.message = args[0]
        else:
            self.message = None
    
    def __str__(self):
        print('calling str')
        if self.message:
            return 'MyCustomError, {0}'.format(self.message)
        else:
            return 'MyCustomError has been raise'
"""
# 自定义异常类 ：字典中的值只能是整数或浮点数，负责抛出异常

class IntFloatValueError(Exception):
    def __init__(self,value):
        self.value = value

    def __str__(self):
        return f"{self.value} is invaild in put ,can only accept integers and floats as its value!"
    

class KeyValueContructError(Exception):
    def __init__(self,key,value):
        self.key = key
        self.value = value
    
    def __str__(self):
        return (f"""keys and values need to be passed as either list or tuple.
                Key: {self.key} (type: {type(self.key)})
                Value: {self.value} (type: {type(self.value)})"""
            )


class CustomIntFloatDict(dict):
    empty_dic={}

    def __init__(self,key=None,value=None):
        if key is None or value is None:
            self.get_dict()
        elif not isinstance(key,(tuple,list,)) or \
             not isinstance(value,(tuple,list)):
            raise KeyValueContructError(key,value)
        else:
            zipped = zip(key,value)
            for k,val in zipped:
                if not isinstance(val,(int,float)):
                    raise IntFloatValueError(val)
                dict.__setitem__(self,k,val)
    
    def get_dict(self):
        return self.empty_dic
    
    def __setitem__(self, key, value):
        if not isinstance(value,(int,float)):
            raise IntFloatValueError(value)
        return dict.__setitem__(self,key,value)

x = sys.argv[1]
y = sys.argv[2]
print(f"{x}+{y} = {float(x) + float(y)}")
print(f"sys.argv = {sys.argv}")
print(f"the file is {sys.argv[0]}")


    
    
