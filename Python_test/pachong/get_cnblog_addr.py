#coding: utf-8
import requests
import re
import csv

# 获取博客园精华分区的第一页博客链接和标题
#1.获取首页源代码 - 请求首页
#2.获取首页中精华分区的链接 - 通过正则获取连接
#3.获取精华分区源代码 - 请求精华分区页面
#4.获取精华分区中博客链接和标题 - 通过正则获取连接和标题
domain = "https://www.cnblogs.com/"
headers = {
    "User-Agent":"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:136.0)"
}


resp = requests.get(domain,headers=headers,verify=False)
resp.encoding ='utf-8'

obj1 = re.compile(r'<ul class="sidenav">.*?<li id="sidenav_pick".*?<a href="(?P<href>.*?)"',re.S)
obj2 = re.compile(r'<article class="post-item".*?href="(?P<blog_addr>.*?)".*?target="_blank">(?P<title>.*?)</a>',re.S)
result1 = obj1.search(resp.text)
# print(result1.group('href'))

child_href = domain + result1.group('href').strip("/") 

child_resp = requests.get(child_href,verify=False)
child_resp.encoding = 'utf-8'
#print(child_resp.text)

result2 = obj2.finditer(child_resp.text)
with open('cnblog_addr.csv','a+') as f:
    
    csvwriter = csv.writer(f)
    for it in result2:
        dct = it.groupdict()
        
        csvwriter.writerow(dct.values())
        
    
print('over')
