import socket
import struct

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('localhost', 12345))

while True:
    data = sock.recv(16)  # sizeof(PlayerState) = 16 bajtów
    if len(data) == 16:
        # Rozpakuj: int32, float, float, float
        id, x, y, z = struct.unpack('ifff', data)
        print(f"📦 Received: id={id}, x={x:.2f}, y={y:.2f}, z={z:.2f}")
    else:
        print(f"⚠️ Unexpected data length: {len(data)} bytes")
        print(f"   Hex: {data.hex()}")