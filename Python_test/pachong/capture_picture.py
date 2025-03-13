#coding: utf-8

import urllib.request
import os
import re
import time

url = r'https://blog.csdn.net/Xiaolan_2001/article/details/138717023'
imgPath = r'/home/linux/share/image'

if not os.path.isdir(imgPath):
    os.mkdir(imgPath)
    
imgHtml = urllib.request.urlopen(url).read().decode('utf-8')

urls = re.findall(r'"objURL":"(.*?)"',imgHtml)

index = 1
for url in urls:
    print("download:",url)
    
    try:
        res = urllib.request.urlopen(url)
        if str(res.status) != '200':
            print('download failed: %s' %url)
            continue
    except Exception as e:
        print('download failed: %s' %url)
    
    filename = os.path.join(imgPath,str(time.time()) + '_' + str(index) + '.jpg')
    with open(filename,'wb') as f:
        f.write(res.read())
        print("download successful\n")
        index+=1
print("download end,download total %s picture" %(index-1))

        