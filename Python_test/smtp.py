# coding: utf-8
import smtplib
from email.mime.text import MIMEText
from email.header import Header
from email.utils import parseaddr, formataddr

# 格式化邮件地址，使邮件地址包含友好的姓名显示
def _format_addr(s):
    name,addr = parseaddr(s)
    return formataddr((Header(name, 'utf-8').encode(),addr))

# 连接到SMTP服务器定义邮件服务器的主机地址，这里使用 163 邮箱的 SMTP 服务器地址
mail_host = 'smtp.163.com'
mail_port = 25 #定义邮件服务器的端口号，25 是 SMTP 协议的默认端口号
mail_from = 'lewtis0706@163.com' #发件人的邮箱地址
mail_to = '1911925932@qq.com'   #收件人的邮箱地址
smtp = smtplib.SMTP()  #创建一个 SMTP 对象，用于与 SMTP 服务器进行通信
smtp.connect(mail_host,mail_port)
smtp.login(mail_from,'SJZQHevYLXAbuNnq') #登陆当前发件邮箱账号，密码是授权码

#创建一个 MIMEText 对象，用于表示邮件的正文内容
msg = MIMEText('hello,send by python', 'plain', 'utf-8')
msg['From'] = _format_addr('Python <%s>' % mail_from)
msg['To'] = _format_addr('管理员 <%s>' % mail_to)

#设置邮件的主题，使用 Header 类对主题进行 UTF-8 编码
msg['Subject'] = Header('来自SMTP的问候...','utf-8').encode() 

# 调用 SMTP 对象的 sendmail 方法，将邮件从发件人发送到收件人
smtp.sendmail(mail_from,mail_to,msg.as_string())
smtp.quit()
