#coding: utf-8

import re

# findall()：匹配字符串中所有符合正则的内容
# lst = re.findall(r"\d+","我的电话是10086，你的电话是10010")
# print(lst)

# finditer(): 匹配字符串中所有符合正则的内容，返回的是迭代器
'''
it = re.finditer(r"\d+","我的电话是10086，你的电话是10010") #返回的是match对象的迭代器
for i in it:
    print(i.group()) # 获取match对象的内容
'''

# search,找到一个结果就返回，返回的是match对象
# s = re.search(r"\d+","我的电话是10086，你的电话是10010")
# print(s.group())

# match 从头开始匹配
# s = re.match(r"/d+","我的电话是10086，你的电话是10010")
# print(s.group())

# 预加载正则表达式
# obj = re.compile(r"\d+")
#ret = obj.finditer("我的电话是10086，你的电话是10010")
#for i in ret:
#    print(i.group())

s = """
<div class='jay'><span id ='1'>Tom</span></div>
<div class='jj'><span id ='2'>Lisa</span></div>
<div class='jolin'><span id ='3'>Joe</span></div>
<div class='tory'><span id ='4'>cook</span></div>
<div class='sylar'><span id ='5'>Lee</span></div>
"""
# obj = re.compile(r"<div class='.*?'><span id ='\d+'>.*?</span></div>",re.S)  #re.S 表示'.'可匹配换行符
# (?P<分组名>正则) 可以单独从正则匹配的内容中进一步提取内容
obj = re.compile(r"<div class='.*?'><span id ='(?P<id>\d+)'>(?P<name>.*?)</span></div>",re.S) # 从匹配的内容中提取内容
result = obj.finditer(s)
for it in result:
    print(it.group("name"))
    print(it.group("id"))
