#coding: utf-8
import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin #拼接url
import time
import os
#获取站长之家“抽象图片”板块的图片
#1.拿到网页首页源代码，获取到子页面的链接
#2.拿到子页面的源代码，获取到图片的下载链接
#3.下载图片

url = 'https://sc.chinaz.com/tupian/chouxiangtupian.html'
resp = requests.get(url)
resp.encoding = 'utf-8'
#print(resp.text)
page = BeautifulSoup(resp.text,"html.parser")
alist = page.find("div",class_="tupian-list com-img-txt-list").find_all("a") #子页面的链接
for a in alist:
    child_href = urljoin(url,a.get("href")) #拼接url，拿到href属性的值，即子页面具体链接，用get方法
    #print(child_href)

    #拿到子页面的源代码
    child_resp = requests.get(child_href)
    child_resp.encoding = 'utf-8'
    #交给BeautifulSoup解析，查找某个标签
    child_page = BeautifulSoup(child_resp.text,'html.parser')
    
    #获取图片的下载链接
    div = child_page.find("div",class_="img-box")
    img = div.find('img')
    img_src = urljoin(child_href,img.get('src'))  #拼接url，拿到src属性的值，即图片下载链接
    
    #下载图片
    img_resp = requests.get(img_src)
    img_content = img_resp.content #拿到图片内容，二进制
    img_name = img_src.split('/')[-1] #图片文件名为图片下载链接的最后一个/后的内容，唯一
    save_path = os.path.join('image',img_name) #保存路径
    with open(save_path,'wb') as f: #以二进制写入
        f.write(img_content)
        
    print(" %s over!" %img_name)
    time.sleep(1)
print("all over!")