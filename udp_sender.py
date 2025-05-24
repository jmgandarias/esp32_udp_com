import socket

UDP_IP = "192.168.1.1"  # IP of your ESP32
UDP_PORT = 1883
MESSAGE = b"Hello from PC!"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print("Press Enter to send a UDP packet. Type 'exit' to quit.")

while True:
    user_input = input(">> ")
    if user_input.lower() == "exit":
        print("Exiting...")
        break
    sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
    print("Packet sent.")
