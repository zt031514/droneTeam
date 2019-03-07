#************************************************
#
# network.py
#
# Author: Zach Tumbleson
#
# Date: 3-5-2019
#
# Description: Implements a simple blob detector
# 			   using openCV functions for image
#			   manipulation and processing
#
#*************************************************

import socket
import pickle

def readin():
	IP = '127.0.0.1'
	PORT = 10000
	BUFFER_SIZE = 8192

	portal = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	portal.bind((IP, PORT))
	portal.listen(1)

	conn, addr = portal.accept()
	print 'Connection address:',addr
	
	
	while 1:
		data = conn.recv(BUFFER_SIZE)
		if not data: break
		print "received data: ", data
		conn.send(data) #echo
	conn.close() 

	print "Data: " + data
	return data