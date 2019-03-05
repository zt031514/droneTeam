#************************************************
#
# firstBlobDetector.py
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

#test imports and compile
print("Hello World")

#read in hot air ballon image
image = cv2.imread("Thermal.jpg", cv2.IMREAD_GRAYSCALE)

cv2.imshow("Input", image)

#try a simple blob detector
detector = cv2.SimpleBlobDetector_create()

#detect blobs
keypoints = detector.detect(image, None)

#Draw detected blobs as red circles
im_with_keypoints = cv2.drawKeypoints(image, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

#display image with blobs circled
cv2.imshow("Keypoints", im_with_keypoints)
cv2.waitKey(0)