#************************************************
#
# main.py
#
# Author: Zach Tumbleson
#
# Date: 3-17-2019
#
# Description: Script to generate test 60x80
#			    thermal images
#
#*************************************************

import numpy as np
import scipy.misc

min = 0
max = 255

width = 80
height = 60

test = [[np.random.randint(min,max) for i in range(width)] for j in range(height)]
scipy.misc.imsave('test.jpg', test)

print test
print "image saved"