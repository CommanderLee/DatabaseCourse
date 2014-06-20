import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('127.0.0.1', 8080))
print "connected"
sock.send("hello world!\r") 
print "send"
recvData = sock.recv(1024)
print "recv data: " + recvData
sock.close()
