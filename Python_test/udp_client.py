# coding: utf-8
import socket,threading

s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

serv_addr = ('127.0.0.1',9999)

def recv_msg():
    while True:   
        msg = s.recv(1024)
        if not msg or msg.decode('utf-8') == 'exit':
            break
        print(f"\n[from server]: {msg.decode('utf-8')}")
        
def send_msg():
    while True:  
        msg = input('[client input] >> ')
        s.sendto(msg.encode(),serv_addr)
        if msg == 'exit':
            break
       
recv_thread = threading.Thread(target=recv_msg)
send_thread = threading.Thread(target=send_msg)

recv_thread.start()
send_thread.start()

recv_thread.join()
send_thread.join()

s.close()