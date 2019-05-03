#************************************************
#
# imageTest.py
#
# Author: Zach Tumbleson
#
# Date: 3-5-2019
#
# Description: Testing the image processing on
#			   test lab images
#
#*************************************************

import imageProcessing as proc
import global_vars
import cv2
import numpy as np

global_vars.init()
path = global_vars.imagesPath + "images_4_24_19/thermal65.jpg"

hotspot, processed = proc.process(path)

print "Hotspot?: " + str(hotspot)
image = cv2.imread(path, cv2.IMREAD_GRAYSCALE)

resized = cv2.resize(src=image, dsize=(640,480), fx = 8, fy = 8,interpolation=cv2.INTER_NEAREST)

processedResize = cv2.resize(src=processed, dsize=(640,480), fx = 8, fy = 8,interpolation=cv2.INTER_NEAREST)
cv2.imshow("Resized Raw",resized)
cv2.imshow("processed", processedResize)
cv2.waitKey(0)
 