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
import threading

def readThermal(sockObj, count, BUFFER_SIZE):

	

	
	print "This is image " + str(count)
	#sockObj.send(startMsg)

	image = np.zeros((60, 80), dtype=int)
	for i in range(60):
		data = sockObj.recv(BUFFER_SIZE)
		image[i] = (proc.process(data, count))
	
	return image

def readVisual(sockObj, count, BUFFER_SIZE):
	
	startMsg = "\x00"
	sockObj.send(startMsg)

	messageLength = int(ord(sockObj.recv(BUFFER_SIZE)))
	print "Image is " + messageLength + "bytes long"

	bytesReceived = 0

	data = ""
	while(bytesReceived < messageLength):
		current = sockObj.recv(BUFFER_SIZE)
		data += current
	print data

