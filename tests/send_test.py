import socket
import struct
import time

MAX_PACKET_SIZE = 1024


HOST = "127.0.0.1"
PORT = 12345

DATA_PACKET_FORMAT = f"<IIII{MAX_PACKET_SIZE}s"
DATA_PACKET_SIZE = struct.calcsize(DATA_PACKET_FORMAT)
HEADER_FORMAT = "<BI"   # B = uint8, I = uint32 (little endian)
HEADER_SIZE = struct.calcsize(HEADER_FORMAT)


def send_data_packet(sock, owner_id, data_type, payload: bytes):
    payload = payload[:MAX_PACKET_SIZE]
    payload_padded = payload.ljust(MAX_PACKET_SIZE, b'\x00')

    timestamp = int(time.time())
    data_size = len(payload)

    # body
    body = struct.pack(
        DATA_PACKET_FORMAT,
        owner_id,
        data_type,
        timestamp,
        data_size,
        payload_padded
    )

    # header
    header = struct.pack(
        HEADER_FORMAT,
        1,                 # packetType = 1 (DataPacket)
        len(body)          # bodySize
    )

    sock.sendall(header + body)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(("127.0.0.1", 12345))  # port z configu

send_data_packet(
    sock,
    owner_id=1,
    data_type=42,
    payload=b"Hello from Python!"
)

time.sleep(15)


# with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
#     s.connect((HOST, PORT))
#     print("Connected")
#
#     s.sendall(packet)
#     print("Packet sent")
#
#     time.sleep(2)  # NIE zamykaj od razu
