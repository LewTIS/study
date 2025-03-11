# coding: utf-8
import socket

# create tcp connection
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

s.connect(('httpbin.org',80))

s.send(b'GET /get HTTP/1.1\r\nHost: httpbin.org\r\nConnection: close\r\n\r\n')

# recive response
buffer = []
while True:
    d = s.recv(1024)
    if d:
        buffer.append(d)
    else:
        break
data = b''.join(buffer)

s.close()
header,body = data.split(b'\r\n\r\n', 1) #按空行切割头与正文（\r\n\r\n 代表行结束。分割head和body）
print(header.decode('utf-8'))

with open('request.json','wb') as f:
    f.write(body)