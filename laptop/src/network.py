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
import numpy as np
import processRaw as proc

def createSocket(TCP_IP, TCP_PORT,  BUFFER_SIZE):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((TCP_IP, TCP_PORT))
	#print "connected!"

	startMsg = "\x00"
	s.send(startMsg)
	return s


def readin(sockObj, count, BUFFER_SIZE):

	image = np.zeros((60, 80), dtype=int)
	for i in range(60):
		data = sockObj.recv(BUFFER_SIZE)
		print("Received line: " + i)
		image[i] = (proc.process(data, count))
	
	#save the image and get the full image path
	filename = proc.saveImage(image, count)

	sockObj.send("\x00")

	return filename