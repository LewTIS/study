# coding: utf-8
import socket

#create tcp connection
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# connect server
s.connect(('127.0.0.1',9999))

# receive
msg = s.recv(1024).decode('utf-8')
print(msg)
# send msg to server

for x in [b'Python', b'PHP']:
    x = b'client:'+ x
    s.send(x)
    try:
        response = s.recv(1024)  
        print(response.decode('utf-8'))
    except ConnectionResetError:
        print("连接已被服务器关闭")
        break

s.send(b'exit')
s.close()