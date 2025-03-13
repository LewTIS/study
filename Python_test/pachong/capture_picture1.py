import urllib.request
import os
import time
from bs4 import BeautifulSoup

url = r'https://blog.csdn.net/weiqifa0/article/details/109831108'
imgPath = r'/home/linux/share/image'

if not os.path.isdir(imgPath):
    os.mkdir(imgPath)

headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3'}
req = urllib.request.Request(url, headers=headers)

try:
    response = urllib.request.urlopen(req)
    html_content = response.read().decode('utf-8')
    soup = BeautifulSoup(html_content, 'html.parser')
    img_tags = soup.find_all('img')
    index = 1
    for img in img_tags:
        img_url = img.get('src')
        if img_url:
            print("download:", img_url)
            try:
                img_req = urllib.request.Request(img_url, headers=headers)
                res = urllib.request.urlopen(img_req)
                if str(res.status) != '200':
                    print('download failed: %s' % img_url)
                    continue
            except Exception as e:
                print('download failed: %s' % img_url)
            filename = os.path.join(imgPath, str(time.time()) + '_' + str(index) + '.jpg')
            with open(filename, 'wb') as f:
                f.write(res.read())
                print("download successful\n")
                index += 1
except Exception as e:
    print(f"An error occurred: {e}")

print("download end,download total %s picture" % (index - 1))
