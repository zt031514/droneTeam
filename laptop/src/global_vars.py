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
import classes

def init():

	global thermalImages
	thermalImages = list()

	global hotspots
	hotspots = list()

	global filenames
	filenames = list()

	global timestamps
	timestamps = list()

	global hotTime
	hotTime = list()

	global hotspotCount
	hotspotCount = 0

	global hotspotID
	hotspotID = list()

	global thermalCount
	thermalCount = 0

	global visualCount
	visualCount = 0
	
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
	missionCurrentTime = 0

	global invalidData
	invalidData = False

	global timerTicks
	timerTicks = 0