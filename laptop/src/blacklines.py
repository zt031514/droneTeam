#***************************************************
#
# blacklines.py
#
# Author: Zach Tumbleson
#
# Date: 3-5-2019
#
# Description: Removes black lines from thermal images
#
#**************************************************

import cv2
from PIL import Image
import numpy as np


filename = "/home/ztumbleson/droneTeam/laptop/images/thermal51.jpg"

imageWithLines = cv2.imread(filename)

imageWithoutLines = np.delete(imageWithLines, list(range(0, imageWithLines.shape[1], 2)), axis = 1)

resizedImage = cv2.resize(imageWithoutLines, (780, 480))
resizedWithLines = cv2.resize(imageWithLines, (780,480))

cv2.imshow("Without Lines", resizedImage)
cv2.imshow("With lines", resizedWithLines)
cv2.waitKey(0)