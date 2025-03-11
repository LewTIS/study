# coding: utf-8
import socket
import threading

def receive_message(sock):
    while True:
        try:
            data = sock.recv(1024)
            if not data:
                break
            print(f'\n[来自服务器]: {data.decode()}')
        except ConnectionResetError:
            break

def send_message(sock):
    while True:
        msg = input('[客户端输入] >> ')
        sock.send(msg.encode())
        if msg == 'exit':
            break

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1', 9999))

print(s.recv(1024).decode())

recv_thread = threading.Thread(target=receive_message, args=(s,))
send_thread = threading.Thread(target=send_message, args=(s,))

recv_thread.start()
send_thread.start()

recv_thread.join()
send_thread.join()
s.close()