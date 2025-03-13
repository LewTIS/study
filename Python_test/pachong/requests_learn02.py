#coding: utf-8
import requests

# 获取翻译的内容

url = 'https://fanyi.baidu.com/sug' # 翻译返回数据的url

word = input("Please enter the English word to be translated:")
data = {
    "kw":word
}
# 发送post请求，发送的数据必须放在字典中，通过data参数传递
resp = requests.post(url,data=data)
print(resp.json()) # 将服务器返回的数据转换为json格式 返回一个字典
resp.close() #关闭resp