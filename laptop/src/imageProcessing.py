#************************************************
#
# imageProc.py
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

import cv2
import numpy as np
import matplotlib.pyplot as plt

def process(image):

	# Setup SimpleBlobDetector parameters.
	params = cv2.SimpleBlobDetector_Params()
 
	# Change thresholds
	params.minThreshold = 50;
	params.maxThreshold = 255;
 
	# Filter by Area.
	params.filterByArea = True
	params.minArea = 50
 
	# Filter by Circularity
	params.filterByCircularity = False
	#params.minCircularity = 0.1
 
	# Filter by Convexity
	params.filterByConvexity = True
	params.minConvexity = 0.01
 
	# Filter by Inertia
	params.filterByInertia = False
	#params.minInertiaRatio = 0.01
 
	# Create a detector with the parameters
	ver = (cv2.__version__).split('.')
	if int(ver[0]) < 3 :
	    detector = cv2.SimpleBlobDetector(params)
	else : 
	    detector = cv2.SimpleBlobDetector_create(params)

	#detect blobs
	keypoints = detector.detect(image, None)

	#Draw detected blobs as red circles
	im_with_keypoints = cv2.drawKeypoints(image, keypoints, \
		np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

	#display image with blobs circled
	#cv2.imshow("Keypoints", im_with_keypoints)

	if(len(keypoints) > 0):
		detected = True
	else:
		detected = False

	return detected, im_with_keypoints