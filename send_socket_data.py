import socket

ip="127.0.0.1"
port = 10000

s = socket.socket()
s.connect((ip,port))
s.send(b"Hello there\r")
s.close()
print("Hello master wayne")
