import sys
import socket

if(len(sys.argv) < 3):
    print 'Usage: python simple_server.py LOCAL_ADDRESS LOCAL_PORT'
    sys.exit(0)

SERVER_IP = sys.argv[1]
SERVER_PORT = int(sys.argv[2])

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print 'Try Binding to {0}:{1}'.format(SERVER_IP, SERVER_PORT)
s.bind((SERVER_IP, SERVER_PORT))
print 'Listen on', SERVER_IP, ":", SERVER_PORT
s.listen(1)
(conn, addr) = s.accept()
print 'Accept connection from', addr
print 'Output'
while True:
    str = conn.recv(1000)
    print str
