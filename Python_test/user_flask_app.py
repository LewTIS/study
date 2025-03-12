# coding: utf-8

from flask import Flask
from flask import request
#1. 主页 home
#2. 登录页 login
#3. 登陆成功欢迎页面，登录失败，提示用户名密码错误

app = Flask(__name__)

#首页
@app.route('/', methods=['GET','POST'])
def home():
    return '<h1>Home</h1><p><a href="/login">去登录</a></p>'

#登录页
@app.route('/login',methods=['GET'])
def login():
    return '''<form action="/login" method="post">
              <p>用户名: <input name="username"></p>
              <p>密码: <input name="password" type="password"></p>
              <p><button type="submit">登录</button></p>
              </form>'''
              
#登陆页处理
@app.route('/login', methods=['POST'])
def do_login():
    #从request对象读取表单内容
    param = request.form
    if(param['username'] == 'yjc' and param['password'] == 'yjc'):
        return 'welcome %s !' % param['username']
    else:
        return 'username or password incorrect'
    pass

if __name__ == '__main__':
    
    app.run('',5000)
