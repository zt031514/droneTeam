import socket

UDP_IP_ADDRESS = "127.0.0.1"
UDP_PORT_NO = 6789

serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverSocket.bind((UDP_IP_ADDRESS, UDP_PORT_NO))

while True:
		data, addr = serverSocket.recvfrom(1024)
		print("Message: ", data)
