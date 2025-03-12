# coding:utf-8

import socket,threading

#create UDP connection
s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

# bind addr port
s.bind(('127.0.0.1',9999))

client_addr = None

print('UDP server is running on %s:%s' %('127.0.0.1',9999))

def recv_msg():
    global client_addr
    while True: 
        data,addr = s.recvfrom(1024)
        client_addr = addr
        if not data or data.decode('utf-8') == exit:
            break
        
        msg = data.decode('utf-8')
        print('Received from %s : %s' %(addr,msg)) 
    
def send_msg():
    while True: 
        if client_addr:
            msg = input('[server input] >> ')
            s.sendto(msg.encode(),client_addr)
            if msg == 'exit':
                break

recv_thread = threading.Thread(target=recv_msg)
send_thread = threading.Thread(target=send_msg)

recv_thread.start()
send_thread.start()

recv_thread.join()
send_thread.join()
    
s.close()
      