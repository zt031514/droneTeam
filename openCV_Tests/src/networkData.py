#************************************************
#
# networkData.py
#
# Author: Zach Tumbleson
#
# Date: 3-5-2019
#
# Description: Processes the data received from
#			   the network, stores it in an array
#			   and saves it as an image file
#
#*************************************************

def process(newtorkData):
	#strip header bytes off of the payload
	singleLine = networkData
	print singleLine + "\n"

	stripped = singleLine[8:]

	print stripped + "\n"