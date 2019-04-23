#************************************************
#
# imageHandler.py
#
# Author: Zach Tumbleson
#
# Date: 3-5-2019
#
# Description: Processes the data received from
#			   the network, stores it in an array
#			   and saves it as an image file, stores
#		       all data captured in a mission
#
#*************************************************

import numpy as np
import scipy.misc
import cv2
import global_vars

#process thermal data from the network and parse into a 2d array
def processThermal(rawData, count):

	#strip header bytes off of the payload
	strippedData = rawData[4:]

	#Separate each pixel into an array element
	pixelList = list(strippedData)

	#declare matrix that is the size of the image
	pixels = np.zeros(80, dtype=int)

	if(len(rawData) < 80):
		global_vars.invalidData = True
		return pixels

	#convert raw line bytes into grayscale ints
	for i in range(80):
		pixels[i] = int(ord(pixelList[i])) & 0xFF
	
	#Return the pure image matrix
	return pixels


def getFilename():
	filename = global_vars.imagesPath + "thermal" + str(global_vars.thermalCount) + ".jpg"
	return filename

def saveImage(imageMatrix):

	filename = global_vars.imagesPath + "thermal" + str(global_vars.thermalCount) + ".jpg"
	cv2.imwrite(filename, imageMatrix)
	return filename

def saveHotspot(image):
	#Save image in hotspot directory
	image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
	new_name = global_vars.hotspotPath + "thermal" + str(global_vars.thermalCount)
	cv2.imwrite(new_name, image)


def saveThermalImages():
	numSaved = 0
	for i in range(len(global_vars.thermalImages)):
		cv2.imwrite(global_vars.filenames[i], global_vars.thermalImages[i])

def saveGPS():
	print "nice"

