# coding:utf-8
import socket
import threading

def handle_client(sock,addr):
    print(f'客户端 {addr} 已连接')
    sock.send(b'server:Welcome!')

    #消息接收
    def recv_msg():
        while True:
            try:
                data = sock.recv(1024)
                if not data or data.decode() == 'exit':
                    break
                print(f'\n[来自{addr}]:  {data.decode()}')
            except ConnectionResetError:
                break
        sock.close()

    #消息发送
    def send_msg():
        while True:
            try:
                msg = input('[服务器输入] >> ')
                sock.send(msg.encode())
            except BrokenPipeError:
                break
    
    recv_thread = threading.Thread(target=recv_msg)
    send_thread = threading.Thread(target=send_msg)

    recv_thread.start()
    send_thread.start()

    recv_thread.join()
    send_thread.join()
    print(f'客户端 {addr} 已断开')

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind(('0.0.0.0',9999))
s.listen(5)

print('服务器启动 (ctrl+c退出)')
while True:
    sock,addr = s.accept() # 返回一个元组，里面有两个元素：socket对象和客户端地址及端口，如(socket,('127.0.0.1',9999))
    threading.Thread(target=handle_client,args=(sock,addr)).start()

