import socket

def add_bytes(bytearray, *args):
    for arg in args:
        bytearray.append(arg)

b = bytearray()

add_bytes(b, 0, 0, 0, 0) # block type
add_bytes(b, 0, 0, 0, 22)
add_bytes(b, 0x69)
add_bytes(b, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88)
add_bytes(b, 4) # length of the string
add_bytes(b, ord('i'), ord('p'), ord('v'), ord('4'))

add_bytes(b, 0, 0, 0, 0xFF) # block type (should be unrecognized and skipped)
add_bytes(b, 0, 0, 0, 8)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(b, ('127.0.0.1', 4040))

b = bytearray()

add_bytes(b, 0, 0, 0, 0) # block type
add_bytes(b, 0, 0, 0, 22)
add_bytes(b, 0x96)
add_bytes(b, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11)
add_bytes(b, 4) # length of the string
add_bytes(b, ord('i'), ord('p'), ord('v'), ord('6'))

sock6 = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock6.sendto(b, ('::', 4040))
