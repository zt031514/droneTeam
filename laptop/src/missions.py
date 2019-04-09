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
import processRaw as proc
import numpy as np
import global_vars

def readThermal(sockObj, BUFFER_SIZE):



	startMsg = "\x00"
	sockObj.send(startMsg)

	print "This is image " + str(global_vars.thermalCount)
	#sockObj.send(startMsg)

	image = np.zeros((60, 80), dtype=int)
	for i in range(60):
		data = sockObj.recv(BUFFER_SIZE)
		image[i] = (proc.processThermal(data, global_vars.thermalCount))
	
	rawTime = sockObj.recv(BUFFER_SIZE)
	timeList = list(rawTime)
	print "Length of timestamp = " + str(len(timeList))
	
	#convert timeList to ints
	timeInts = np.zeros(len(timeList))
	timestamp = 0
	for i in range(len(timeList)):
		timeInts[i] = int(ord(timeList[i])) & 0xFF
		timestamp += timeInts[i]


	return image, timestamp

def readVisual(sockObj, BUFFER_SIZE):
	
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

	return data


def thermalMission(s, BUFFER_SIZE):

	#Thermal image loop
	imageFiles = [""]

	image, timestamp = readThermal(s, BUFFER_SIZE)
	print image
	print "Timestamp = " + str(timestamp)
	
	#save the image and get the full image path
	filename = proc.saveImage(image)
	global_vars.thermalCount = global_vars.thermalCount + 1

	return filename


def visualMission(s, BUFFER_SIZE):

	imageFiles = [""]
	filename = ""
	for i in range(30):
		print "Saved: " + filename + ", moving on."
		print image
	
	global_vars.visualCount = global_vars.visualCount + 1

	return filename