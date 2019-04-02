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
import network as net
import processRaw as proc
import numpy as np
import cv2

#keep track of the number of thermal images stored so far
thermalCount = 0


TCP_IP = '10.0.0.2'
TCP_PORT = 500
BUFFER_SIZE = 164

s = net.createSocket(TCP_IP, TCP_PORT, BUFFER_SIZE)

#reads the image in from the network
#calls helper functions process and saveImage
#to save the image and returns the filename of the 
#saved image
for i in range(10):
	filename = net.readin(s, thermalCount, BUFFER_SIZE)
	print "Saved: " + filename + ", moving on."
	thermalCount = thermalCount + 1

s.send("\xFF")

#image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
#cv2.imshow(filename, image)
#cv2.waitKey(0)
print "Done."
#imgDir = "/home/ztumbleson/droneTeam/openCV_Tests/images/"

#success = ip.process(filename)

#if success == 0:
#	print("Image Processing Complete.")

s.close()