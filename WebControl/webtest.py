import socket
import time

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
address = ('127.0.0.1', 27015) 
   
s.connect(address)  

s.send(b"123456")
time.sleep(0.1)
s.send(b"closemusic")

s.close()
