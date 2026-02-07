import socket
import struct
import time

HOST = "127.0.0.1"
PORT = 12345

# ===== PlayerState =====
player_id = 1
x, y, z = 10.0, 20.0, 30.0

packet_type = 1

# body: int32 + 3 float32
body = struct.pack("<ifff", player_id, x, y, z)  # 16 bajtów

# header: uint8 + uint32 (little endian)
header = struct.pack("<BI", packet_type, len(body))  # 1 + 4 = 5 bajtów

packet = header + body

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    print("Connected")

    s.sendall(packet)
    print("Packet sent")

    time.sleep(2)  # NIE zamykaj od razu
