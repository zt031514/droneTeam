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


def process(rawData, count):
	#if(len(rawData) < 80):
	#	print rawData
	#	print "Faulty Data Received"
	#	return 0

	#strip header bytes off of the payload
	strippedData = rawData[4:]

	#Separate each pixel into an array element
	pixelList = list(strippedData)
	#print pixelList
	#print len(pixelList)
	


	#declare matrix that is the size of the image
	pixels = np.zeros(80, dtype=int)

	for i in range(80):
		pixels[i] = int(ord(pixelList[i])) & 0xFF;

	#Return the pure image matrix
	return pixels

def saveImage(imageMatrix, count):
	
	imgDir = "/home/ztumbleson/droneTeam/laptop/images/"

	filename = imgDir + "thermal" + str(count) + ".jpg"
	cv2.imwrite(filename, imageMatrix)
	return filename