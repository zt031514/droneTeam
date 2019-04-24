#************************************************
#
# classes.py
#
# Author: Zach Tumbleson
#
# Date: 3-5-2019
#
# Description: Defines the thermalImage
#			   and GPS coordinate classes
#
#*************************************************

#Store gps coordinate as a latitude and longitude
class Gps_Coord(object):
	def __init__(self, latitude=0, longitude=0):
		self.latitude = latitude
		self.longitude = longitude


#store an image and a gps tag
class ThermalImage(object):
	def __init__(self, image = None, gps = None):
		self.image = image
		self.gps = gps
