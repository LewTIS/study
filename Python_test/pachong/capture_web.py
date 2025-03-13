# coding: utf-8
import urllib.request

url =  'https://blog.csdn.net/shanandqiu/article/details/115351654'

with urllib.request.urlopen(url) as f:
    headers = f.getheaders()
    body = f.read()
    
    print(f.status,f.reason)
    for k,v in headers:
        print(k + ': ' + v)
    
    html_content = body.decode('utf-8')
    with open('csdn_blog.html','w') as file:
        file.write(html_content)
    
    