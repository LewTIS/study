#coding : utf-8
import requests
import re
import csv

#1.拿到页面源代码，requests
#2.用正则提取数据

url = "https://movie.douban.com/top250"
params = {
    "start":75
}

headers = {
    "User-Agent":"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:136.0)"
}

resp = requests.get(url,headers=headers,params=params)
page_content = resp.text # 获取网页源代码
resp.close()

pattern = (
    r'<li>.*?<div class="item">.*?<span class="title">(?P<name>.*?)'
    r'</span>.*?<p>.*?<br>(?P<year>.*?)&nbsp.*?property="v:average">(?P<score>.*?)'
    r'</span>.*?<span>(?P<num>.*?人评价)</span>'
) 
obj = re.compile(pattern,re.S)

result = obj.finditer(page_content)
# 将结果写入csv文件
with open('douab_top250.csv','a+') as f:
    csvwriter = csv.writer(f)
    for it in result:
        dct = it.groupdict() # 将匹配的内容转换为字典
        dct['year'] = dct['year'].strip()
        csvwriter.writerow(dct.values())
    
print("over")


    