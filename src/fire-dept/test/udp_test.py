import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto('Hello, World!', ('127.0.0.1', 1234))