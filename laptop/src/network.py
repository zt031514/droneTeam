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

def readThermal(sockObj, count, BUFFER_SIZE):

	startMsg = "\x00"

	sockObj.send(startMsg)

	image = np.zeros((60, 80), dtype=int)
	for i in range(60):
		data = sockObj.recv(BUFFER_SIZE)
		image[i] = (proc.process(data, count))
	
	#save the image and get the full image path
	filename = proc.saveImage(image, count)

	return filename, image

def readVisual(sockObj, BUFFER_SIZE):
	
	startMsg = "\x00"

	sockObj.recv(BUFFER_SIZE)
