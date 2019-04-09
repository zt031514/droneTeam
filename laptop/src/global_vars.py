#************************************************
#
# globals.py
#
# Author: Zach Tumbleson
#
# Date: 4-4-2019
#
# Description: Stores global stopThermal for use between ui and missions files
#
#*************************************************


import time

def init():
	global hotspotCount
	hotspotCount = 0

	global thermalCount
	thermalCount = 0

	global visualCount
	visualCount = 0

	global hotspotCount
	hotspotCount = 0
	
	global stopThermal
	stopThermal = False

	global stopVisual
	stopVisual = False

	global fullStop
	fullStop = False

	global imagesPath
	imagesPath = "/home/ztumbleson/droneTeam/laptop/images/"

	global hotspotPath
	hotspotPath = "/home/ztumbleson/droneTeam/laptop/images/hotspots/"

	global missionStartTime
	missionStartTime = time.time()

	global missionCurrentTime
	missionCurrentTime = time.time()