#coding: utf-8
from urllib.request import urlopen

url = "http://www.baidu.com"
resp = urlopen(url)


with open("mybaidu.html",'w') as f:
    f.write(resp.read().decode('utf-8'))
print('over')