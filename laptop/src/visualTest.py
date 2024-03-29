#************************************************
#
# main.py
#
# Author: Zach Tumbleson
#
# Date: 3-5-2019
#
# Description: main module implementing the 
#			   software for the drone project
#
#*************************************************

import imageProcessing as ip
import socket
import network as net
import processRaw as proc
import numpy as np
import cv2
import threading

def readThermal(sockObj, count, BUFFER_SIZE):

	startMsg = "\x00"
	s.send(startMsg)

	print "This is image " + str(count)
	#sockObj.send(startMsg)

	image = np.zeros((60, 80), dtype=int)
	for i in range(60):
		data = sockObj.recv(BUFFER_SIZE)
		image[i] = (proc.process(data, count))
	
	return image


#keep track of the number of thermal images stored so far
thermalCount = 0

TCP_IP = '10.0.0.2'
TCP_PORT = 500
BUFFER_SIZE = 164

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

#Thermal image loop
imageFiles = [""]
while(1):
	image = readThermal(s, thermalCount, BUFFER_SIZE)
	#print "Saved: " + filename + ", moving on."
	print image
	
	#save the image and get the full image path
	filename = proc.saveImage(image, thermalCount)
	thermalCount = thermalCount + 1

#s.send("\xFF")


#imgDir = "/home/ztumbleson/droneTeam/openCV_Tests/images/"

s.close()