#coding: utf-8
import requests

query = input('Please input your favorite star:')

url = f'https://www.sogou.com/web?query={query}'
headers = {
    "User-Agent": "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:136.0) Gecko/20100101 Firefox/136.0"
}
resp = requests.get(url,headers=headers) #发送请求，添加headers，处理反爬

print(resp)
#print(resp.text) # 获取网页源代码
with open('search_star.html','w') as f:
    f.write(resp.text)