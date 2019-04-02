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

#keep track of the number of thermal images stored so far
thermalCount = 1

TCP_IP = '10.0.0.2'
TCP_PORT = 500
BUFFER_SIZE = 164

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

#reads the image in from the network
#calls helper functions process and saveImage
#to save the image and returns the filename of the 
#saved image
filename, image = net.readThermal(s, thermalCount, BUFFER_SIZE)
print "Saved: " + filename + ", moving on."
print image
thermalCount = thermalCount + 1

#s.send("\xFF")


#imgDir = "/home/ztumbleson/droneTeam/openCV_Tests/images/"

s.close()

#pathToImages = "/home/ztumbleson/droneTeam/openCV_Tests/images/"
#image = "test2.jpeg"

#path = pathToImages + image
#print path
#success = ip.process(path)

#if success == 0:
#	print("Image Processing Complete.")

#networkData = net.readin()

#image = nd.process(networkData)
