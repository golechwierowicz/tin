import socket

def add_bytes(bytearray, *args):
    for arg in args:
        bytearray.append(arg)

b = bytearray()

add_bytes(b, 0, 0, 0, 0xFF) # block type (should be unrecognized and skipped)
add_bytes(b, 0, 0, 0, 8)

add_bytes(b, 0, 0, 0, 6) # heartbeat
add_bytes(b, 0, 0, 0, 12)
add_bytes(b, 0, 0, 0, 123) # id

add_bytes(b, 0, 0, 0, 5) # fire alert
add_bytes(b, 0, 0, 0, 24)
add_bytes(b, 0, 0, 0, 0, 0, 0, 0, 1) # timestamp
add_bytes(b, 0, 2) # latitude
add_bytes(b, 0, 3) # longitude
add_bytes(b, 0, 0, 0, 4) # alerts count

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(b, ('127.0.0.1', 4099))
