#coding; utf-8
import requests

# 获取豆瓣喜剧排行榜
#url = 'https://movie.douban.com/j/chart/top_list?type=24&interval_id=100:90&action=&start=0&limit=20' # '?'后面是参数，可将参数提取出来
url = 'https://movie.douban.com/j/chart/top_list'
params = {
    "type": 24,
    "interval_id": "100:90",
    "action": "",
    "start": 0,
    "limit": 20,
}
headers = {
    "User-Agent":"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:136.0) Gecko/20100101 Firefox/136.0"
}
resp = requests.get(url=url, params=params,headers=headers)

#print(resp.json())

with open('douban_movie.html','w') as f:
    f.write(resp.text)
resp.close() #关闭resp

