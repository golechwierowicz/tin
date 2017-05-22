import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto('ipv4: Test Message', ('127.0.0.1', 1234))

sock6 = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock6.sendto('ipv6: Test Message', ('::', 1234))
