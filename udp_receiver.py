import socket
import time

UDP_IP = "0.0.0.0"  # Listen on all interfaces
UDP_PORT = 1234     # Must match the ESP32 sender port

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
sock.setblocking(False)  # Non-blocking mode

print(f"Listening on UDP port {UDP_PORT}...")

packet_count = 0
start_time = time.time()

try:
    while True:
        try:
            data, addr = sock.recvfrom(1024)  # Buffer size
            packet_count += 1
        except BlockingIOError:
            pass  # No data received

        # Print stats every second
        if time.time() - start_time >= 1.0:
            print(f"Packets received in last second: {packet_count}")
            packet_count = 0
            start_time = time.time()

except KeyboardInterrupt:
    print("Receiver stopped.")
