#************************************************
#
# networkData.py
#
# Author: Zach Tumbleson
#
# Date: 3-5-2019
#
# Description: Processes the data received from
#			   the network, stores it in an array
#			   and saves it as an image file
#
#*************************************************

import numpy as np
import scipy.misc
import cv2
import global_vars


def processThermal(rawData, count):

	#strip header bytes off of the payload
	strippedData = rawData[4:]

	#Separate each pixel into an array element
	pixelList = list(strippedData)

	#declare matrix that is the size of the image
	pixels = np.zeros(80, dtype=int)

	#convert raw line bytes into grayscale ints
	for i in range(80):
		pixels[i] = int(ord(pixelList[i])) & 0xFF
	
	#Return the pure image matrix
	return pixels

def saveImage(imageMatrix):

	filename = global_vars.imagesPath + "thermal" + str(global_vars.thermalCount) + ".jpg"
	cv2.imwrite(filename, imageMatrix)
	return filename