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
from PIL import Image

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
		cv2.imwrite(global_vars.filenames[i], global_vars.thermalImages[i].image)

def saveGPS():
	file = open('gps.txt', 'wb')
	file.write("GPS coordinates\n\n")
	for i in range(len(global_vars.thermalImages)):
		file.write("Thermal Image " + str(i) + '\n')
		file.write("Latitude: " + \
			 str(global_vars.thermalImages[i].gps.latitude) + "\n")
		file.write("Longitude: " + \
			 str(global_vars.thermalImages[i].gps.longitude))
		file.write("\n\n")

	file.close()

def saveHotspots():
	file = open('hotspotData.txt', 'wb')
	file.write("Hotspot data\n\n")
	file.write("Number of hotspots detected: " + str(global_vars.hotspotCount))
	file.write("\n\nHotspot GPS locations\n\n")

	print "Number of hotspots: " + str(global_vars.hotspotCount) + "\n"
	for i in range(global_vars.hotspotCount):
		print "Corresponds to thermal image " + str(global_vars.hotspotID[i]) + "\n"
		print "Timestamp: " + str(global_vars.hotTime[i]) + "\n"

	#Save GPS for hotspots
	for i in range(global_vars.hotspotCount):
		file.write("Hotspot  " + str(i) + '\n')
		file.write("Latitude: " + \
			 str(global_vars.thermalImages[global_vars.hotspotID[i]].gps.latitude) + "\n")
		file.write("Longitude: " + \
			 str(global_vars.thermalImages[global_vars.hotspotID[i]].gps.longitude))
		file.write("\n\n")

	#Save images 
	for i in range(len(global_vars.hotspots)):
		filename = global_vars.hotspotPath + "hotspot" + str(i) + ".jpg"
		cv2.imwrite(filename, global_vars.hotspots[i])

	file.close()

def removeLines(image):

	imageWithoutLines = np.delete(image, list(range(0, image.shape[1], 2)), axis = 1)

	return imageWithoutLines
