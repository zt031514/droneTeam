import socket
import sys

UDP_IP_ADDRESS = "127.0.0.1"
UDP_PORT_NO = 6789
Message = "Hello, Server"

clientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

clientSocket.sendto(Message, (UDP_IP_ADDRESS, UDP_PORT_NO))
