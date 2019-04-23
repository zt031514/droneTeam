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
import select

def flushSocket(sockObj):
	
	print "Flushing socket."
	sockObj.setblocking(0)
	input = [sockObj]

	while 1:
		inputready, o, e = select.select(input,[],[], 0.0)
		if len(inputready)==0: break
		for s in inputready: s.recv(1)
	sockObj.setblocking(1)
	global_vars.invalidData = False

def readThermal(sockObj, BUFFER_SIZE):

	if(global_vars.stopThermal == True):
		return np.zeros((60, 80), dtype = int)

	startMsg = "\x00"
	sockObj.send(startMsg)

	#print "This is image " + str(global_vars.thermalCount)

	image = np.zeros((60, 80), dtype=int)
	for i in range(60):
		data = sockObj.recv(BUFFER_SIZE)
		image[i] = (proc.processThermal(data, global_vars.thermalCount))

	rawTime = sockObj.recv(BUFFER_SIZE)
	timeList = list(rawTime)
	
	#convert timeList to ints
	timeInts = np.zeros(len(timeList))
	timestamp = 0
	for i in range(len(timeList)):
		timeInts[i] = int(ord(timeList[i])) << (8*i) & 0xFF
		timestamp += timeInts[i]

	return image, timestamp

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

def readVisual(sockObj, BUFFER_SIZE):
	
	imageInfo = sockObj.recv(BUFFER_SIZE)

	numBytes = imageInfo[:3]
	numPackages = imageInfo[4:] 

	numBytes_High = int(ord(numBytes[0])) << 16 & 0xFF0000
	numBytes_Middle = int(ord(numBytes[1])) << 8 & 0xFF00
	numBytes_Low = int(ord(numBytes[2])) & 0xFF

	numBytes = numBytes_High + numBytes_Middle + numBytes_Low

	print "Image is " + str(numBytes) + " bytes long"

	bytesReceived = 0

	data = ""
	
	while(bytesReceived < numBytes):
		current = sockObj.recv(BUFFER_SIZE)
		data += current
		bytesReceived = len(data)
		print str(bytesReceived) + " bytes received so far."
	print "Image received."


	return data


def visualMission(s, BUFFER_SIZE):
	
	image = readVisual(s, BUFFER_SIZE)

	filename = global_vars.imagesPath + "visual" + \
			str(global_vars.visualCount) + ".jpg"
	file = open(filename, "wb+")
	file.write(image)
	file.close()

	global_vars.visualCount = global_vars.visualCount + 1
	
	return filename