import socket
import time

UDP_IP = "0.0.0.0"  # Listen on all interfaces
UDP_PORT = 1883     # Must match the ESP32 sender port

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP packets on port {UDP_PORT}...")

last_time = None

try:
    while True:
        data, addr = sock.recvfrom(1024)  # Blocking call
        current_time = time.time()

        if last_time is not None:
            elapsed_ms = (current_time - last_time) * 1000
            print(f"Elapsed time: {elapsed_ms:.2f} ms")

        print(f"Received from {addr}: {data.decode(errors='ignore')}")
        last_time = current_time

except KeyboardInterrupt:
    print("\nReceiver stopped.")
