#coding: utf-8
import requests
from lxml import etree

url = 'https://www.zbj.com/fw/?k=saas'
resp = requests.get(url)
f = open('zhubajir.html','w')
f.write(resp.text)
html = etree.HTML(resp.text)


divs = html.xpath('//div[@class="service-card-wrap"]')

for div in divs:
    price = div.xpath('.//div[@class="price"]/span/text()')
    title = div.xpath('.//div[@class="name-pic-box"]/a/span/text()')
    name = div.xpath('.//div[@class="shop-box"]/div/div/text()')
    print(price,title,name)
    
                        

