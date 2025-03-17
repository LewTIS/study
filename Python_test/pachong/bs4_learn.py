#coding" utf-8
#通过 beautifulsoup 爬取 nba 球员数据榜单

import requests
from bs4 import BeautifulSoup
import csv
#1.获取网页内容
url = 'https://nba.hupu.com/stats/players/pts'
resp = requests.get(url)

#2.通过 BeautifulSoup 解析网页，生成 BeautifulSoup 对象
page = BeautifulSoup(resp.text)
#3.从 BeautifulSoup 对象中获取表格数据
#find("标签名"，属性=值) 类似<div class="players_table">  div是标签名 class是属性
#findall("标签名",属性=值)
table = page.find("table",class_ ="players_table")

#4.获取表格每一行
trs = table.find_all("tr")
with open("record.csv","w") as f:
    csvwriter = csv.writer(f)
    for tr in trs: 
        tds = tr.find_all("td") #获取每一行的具体数据
        ranking = tds[0].text #tds[0].text表示获取被标签标记的内容
        player = tds[1].text
        team = tds[2].text
        score = tds[3].text
        hit = tds[4].text
        hit_rate = tds[5].text
        hit_three = tds[6].text
        hit_rate_three = tds[7].text
        free_throw = tds[8].text
        free_throw_rate = tds[9].text
        session = tds[10].text
        play_time = tds[11].text
        csvwriter.writerow([ranking,player,team,score,hit,hit_rate,hit_three,hit_rate_three,free_throw,free_throw_rate,session,play_time])
    

print('over')