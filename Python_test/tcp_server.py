# coding: utf-8
import socket,threading

# create tcp connection
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

# bind ip and port
s.bind(('127.0.0.1',9999))

#monitor client connection

s.listen(3) # Waiting for maximum number of connections

print('Server is running on %s:%s' %('127.0.0.1',9999))
print('Waiting for connection...')
def tcp_link(sock,addr):
    print('Accept new connect from %s:%s...' %addr)
    sock.send(b'server:Welcome!')

    while True:
        r = sock.recv(1024)
        if not r or r.decode('utf-8') == 'exit': #end connection instruction
            break
        
        #msg = addr + (r.decode('utf-8'),)
        msg = r.decode('utf-8')
        print('%s' %msg)
        
        #reply client
        response = f'server:{msg}'.encode('utf-8')
        sock.send(response)
    sock.close()
    print('client %s:%s is closed.' % addr)

while True:
    
    sock,addr = s.accept() 
    print(addr) #('127.0.0.1',62090)      
    
    t = threading.Thread(target=tcp_link,args=(sock,addr))
    t.start()
    
