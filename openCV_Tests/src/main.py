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
import networkData as nd

pathToImages = "/home/ztumbleson/droneTeam/openCV_Tests/images/"
image = "test2.jpeg"

path = pathToImages + image
print path
success = ip.process(path)

if success == 0:
	print("Image Processing Complete.")

#networkData = net.readin()

#image = nd.process(networkData)
