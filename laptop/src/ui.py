#************************************************
#
# imageProc.py
#
# Author: Zach Tumbleson
#
# Date: 3-17-2019
#
# Description: Starter gui, features to added over
#			   time.
#
#*************************************************

try:
    # for Python2
    import Tkinter as tk
except ImportError:
    # for Python3
    import tkinter as tk

from PIL import ImageTk, Image
import missions
import global_vars
import socket
import cv2
import imageProcessing as improc
import time
import imageHandler as proc
import classes
import numpy
#import folium


#*************
#
#Actions
#
#*************


#**********************************************
#
# Called when "Start Thermal" button is pressed
#
#***********************************************
def thermalButton(top, imgCanvas):


	#NETWORK CONSTANTS
	TCP_IP = '10.0.0.2'
	TCP_PORT = 500
	BUFFER_SIZE = 164


	#Open Socket
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((TCP_IP, TCP_PORT))


	#Begin thermal mission code
	
	#Stop button has not been pressed
	global_vars.stopThermal = False

	#No hotspot has been detected for the current image
	hotspot = False

	#While the stop button has not been pressed, do the loop
	while(global_vars.stopThermal == False):
		
		#If PIC sends a bad picture, flush the rest of the image
		if(global_vars.invalidData == True):
			missions.flushSocket(s)


		#Request a thermal image from the PIC and receive the image, the gps
		#coordinates and a filename
		image, coordinates, filename = missions.thermalMission(s, BUFFER_SIZE)


		#Create a new thermalImage object
		thermalImage = classes.ThermalImage()

		#remove lines from the received image and store
		thermalImage.image = proc.removeLines(image)

		#store gps coordinates
		thermalImage.gps = coordinates

		#Add the thermal image and filename to the appropriate structures
		global_vars.thermalImages.append(thermalImage)
		global_vars.filenames.append(filename)

		#Determine if the image contains a hotspot
		#hotspot, processedImage = improc.process(filename)
		
		#Hotspot detected, update GUI and counter
		if(hotspot):
			
			#Add to the hotspot counter, add the image to the array and 
			#add the thermal counter for later use
			global_vars.hotspotCount = global_vars.hotspotCount + 1
			global_vars.hotspots.append(processedImage)
			global_vars.hotspotID.append(global_vars.thermalCount)

		#print the thermal image to the window in the ui

		#Convert the raw image data to a PIL image object, encode as uint8
		displayImage = Image.fromarray((thermalImage.image).astype('uint8'))

		#resize from 80x60 to 640x480 using nearest neighbor interpolation
		resizedImage = displayImage.resize((640,480), Image.NEAREST)

		#Create a tkinter compatible object from the resized image
		displayReadyImage = ImageTk.PhotoImage(resizedImage)

		#Put the image in the tk canvas
		image = imgCanvas.create_image(400, 250, image=displayReadyImage)

		#TODO ADD STATS UPDATE CODE
	
		#TODO ADD LIST UPDATE CODE

		#Update the current thermal image number
		global_vars.hotspotCount = global_vars.hotspotCount + 1

		top.update_idletasks()
		top.update()


	#Stop button has been pressed, exit to top loop
	print str(global_vars.thermalCount) + " images received"

	s.close()

#**********************************************
#
# Called when "Start Visual" button is pressed
#
#***********************************************
def visualButton(top, imgCanvas):

	TCP_IP = '10.0.0.2'
	TCP_PORT = 500
	BUFFER_SIZE = 164

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((TCP_IP, TCP_PORT))

	startMsg = "\x00\x05\x0a\x0e\x14\x19\x2d\x32\x36\x3a"
	s.send(startMsg)	

	global_vars.stopVisual = False

	while(global_vars.visualCount < 1):
		top.update_idletasks()
		top.update()
		print "Reading in image " + str(global_vars.visualCount)
		path = missions.visualMission(s, BUFFER_SIZE)

	while(global_vars.stopVisual == False):
		rawImage = Image.open(path)

		resizedImage = rawImage.resize((780,480), Image.NEARES)

		displayReadyImage = ImageTk.PhotoImage(resizedImage)

		image = imgCanvas.create_image(400,250, image=displayReadyImage)

		top.update_idletasks()
		top.update()
	print str(global_vars.visualCount) + " images received"
	global_vars.visualCount = 0

	s.close()

#**********************************************
#
# Called when "Stop Thermal" button is pressed
#
#***********************************************
def thermalStop():
	global_vars.stopThermal = True


#**********************************************
#
# Called when "Stop Visual" button is pressed
#
#***********************************************
def visualStop():
	global_vars.stopVisual = True

#**********************************************
#
# Called when "Exit" button is pressed
#
#***********************************************
def fullStop():
	global_vars.fullStop = True

	proc.saveThermalImages()
	proc.saveGPS()

#**********************
#
# END ACTIONS
#
#**********************

#**********************
#
# INIT GLOBAL VARS
#
#**********************
global_vars.init()


top = tk.Tk()
top.geometry('1000x1000')
top.title('Dronez')
top.pack_propagate(0)

#*********************
#
# WINDOW
#
#*********************


#*************************
#
# COLOR CODING
#
#*************************

#Left frame is blue
#left Top is Red
#Left middle is Brown
#Left bottom is purple

#Right Frame is blue
#Right top is green
#Right bottom is yellow

#*********************
#
# Left Frame
#
#	Contains buttons, mission stats, and hotspot list
#
#*********************

leftFrame = tk.Frame(top, height = 1000, width = 200, bg = "blue")
leftFrame.pack(side = tk.LEFT)


#*****************************
#
# Left Top Frame
#
# Contains Start/Stop Buttons
#
#******************************

leftTop = tk.Frame(leftFrame, height = 300, width = 200, bg = "Red")
leftTop.pack_propagate(0)
leftTop.pack(fill=tk.BOTH)

#****************************************
#
# Left Middle Frame
#
# Contains Mission Stats
#
#*****************************************

#Declare frame size and geometry
leftMiddle = tk.Frame(leftFrame, height = 300, width = 200, bg = "gray", borderwidth = 5, relief = tk.SUNKEN)
leftMiddle.pack_propagate(0)
leftMiddle.pack(fill=tk.BOTH)

#TODO: ADD Mission Stats

#Mission Stats text
statsTitle = tk.StringVar()
statsLabel = tk.Label(leftMiddle, bg = "gray", \
	textvariable = statsTitle)
statsTitle.set("Mission Stats\n\n")
statsLabel.pack()

#Timer Text and elapsed time
#"Elapsed Time" text
timerTitle = tk.StringVar()
timerTitleLabel = tk.Label(leftMiddle, bg = "gray", \
	textvariable = timerTitle)
timerTitle.set("Elapsed Time\n")
timerTitleLabel.pack()

#Elapsed time value
timer = tk.StringVar()
timerLabel = tk.Label(leftMiddle, bg = "gray", \
	textvariable = timer)
timerLabel.pack()

#Hotspot counter text and value
#"Hotspots detected counter"
hotspotTitle = tk.StringVar()
hotspotTitleLabel = tk.Label(leftMiddle, bg = "gray", \
	textvariable = hotspotTitle)
hotspotTitle.set("Hotspots Detected\n")
hotspotTitleLabel.pack()

#hotspot counter value
hotspotCount = tk.StringVar()
hotspotCountLabel = tk.Label(leftMiddle, bg = "gray", \
	textvariable = hotspotCount)
hotspotCountLabel.pack()

#GPS text and position
gpsTitle = tk.StringVar()
gpsTitleLabel = tk.Label(leftMiddle, bg = "gray", \
	textvariable = gpsTitle)
gpsTitle.set("Current GPS coordinates\n")
gpsTitleLabel.pack()

#Elapsed time value
latitude = tk.StringVar()
longitude = tk.StringVar()
latitudeLabel = tk.Label(leftMiddle, bg = "gray", \
	textvariable = latitude)
longitudeLabel = tk.Label(leftMiddle, bg = "gray", \
	textvariable = longitude)
latitudeLabel.pack()
longitudeLabel.pack()



#*****************************************
#
# Left Bottom Frame
#
# Contains Hotspot list
#
#*****************************************

#TODO: ADD hotspot list


leftBottom = tk.Frame(leftFrame, height = 400, width = 200, bg = "gray")
leftBottom.pack_propagate(0)
leftBottom.pack(fill=tk.BOTH)

noHotspots = tk.StringVar()
hotspotLabel = tk.Label(leftBottom, bg = "gray" \
			, textvariable = noHotspots)
noHotspots.set("           No Hotspots found.")
hotspotLabel.pack(side = tk.LEFT)


#**********************************
#
# Right Frame
#
# Contains image display and map
#
#**********************************

rightFrame = tk.Frame(top, height = 1000, width = 800, bg = "black")
rightFrame.pack(side = tk.RIGHT, fill = tk.BOTH)

#******************************************************
#
# Top Right Frame
#
# Contains image canvas for thermal and visual images
#
#******************************************************
rightTop = tk.Frame(rightFrame, height = 500, width = 800, bg = "black")
rightTop.pack_propagate(0)
rightTop.pack(side = tk.TOP)

#Thermal Image canvas, display the start image
imgCanvas = tk.Canvas(rightTop, bg = "black", height = 500, width = 800)
imgCanvas.pack(fill=tk.BOTH)

startImage = Image.open(global_vars.imagesPath + "static/start.png")
resizedImage = startImage.resize((640, 480), Image.NEAREST)
displayReadyImage = ImageTk.PhotoImage(resizedImage)

image = imgCanvas.create_image(0, 0, image=displayReadyImage)

#*******************************************************
#
# Bottom Right Frame
#
# Contains Map canvas
#
#*******************************************************
rightBottom = tk.Frame(rightFrame, height = 500, width = 800, bg = "black")
rightTop.pack_propagate(0)
rightBottom.pack(side = tk.BOTTOM)


#TODO: ADD REAL MAP

#Map canvas
mapCanvas = tk.Canvas(rightBottom, bg = "black", height = 500, width = 800)
mapCanvas.pack(fill = tk.BOTH)
mapImg = Image.open(global_vars.imagesPath + "static/map.png")
resizedImage = mapImg.resize((780, 480), Image.NEAREST)
displayReadyImage = ImageTk.PhotoImage(resizedImage)
mapBox = mapCanvas.create_image(400, 250, image=displayReadyImage)


#******************
#
#Buttons
#
#******************

startThermalButton = tk.Button(leftTop, height = 3, text = "Start Thermal", command = lambda: thermalButton(top, imgCanvas))
startThermalButton.pack(fill = tk.X, side = tk.TOP)

startVisualButton = tk.Button(leftTop, height = 3, command= lambda: visualButton(top, imgCanvas), text = "Start Visual")
startVisualButton.pack(fill = tk.X, side = tk.TOP)

stopThermalButton = tk.Button(leftTop, height = 3, command = lambda: thermalStop(), text = "Stop Thermal")
stopThermalButton.pack(fill = tk.X, side = tk.TOP)

stopVisualButton = tk.Button(leftTop, height = 3, command = lambda: visualStop(), text = "Stop Visual")
stopVisualButton.pack(fill = tk.X, side = tk.TOP)

exitButton = tk.Button(leftTop, height = 5, command = lambda: fullStop(), text = "Exit")
exitButton.pack(fill = tk.X)

#*********************
#
# END BUTTONS
#
#*********************

#************************
#
# END WINDOW
#
#************************

#************************
#
# Top Loop
#
#************************

#while the "Exit" button has not been pressed
while global_vars.fullStop == False:

	global_vars.missionCurrentTime = time.time()
	
	#get the elapsed time since the mission start in seconds
	elapsed= int(global_vars.missionCurrentTime - global_vars.missionStartTime)
	
	#Convert the seconds to a human readable elapsed time string
	elaps_min = elapsed / 60
	elaps_sec = elapsed % 60

	#Form a string containing the minutes and seconds for display
	
	#For the first ten seconds of each minute, print an extra 
	#zero to fill the space
	if(elaps_sec < 10):
		time_string = str(elaps_min) + ":" + "0" + str(elaps_sec) + "\n"
	else:
		time_string = (str(elaps_min) + ":" + str(elaps_sec) + "\n")
	timer.set(time_string)

	#Update the number of hotspots found
	hotspotCount.set(str(global_vars.hotspotCount) + "\n") 

	#Update the curretn GPS position
	latitude.set("Lat: " + str(0))
	longitude.set("Long: " + str(0))

	
	if(global_vars.thermalCount > 1):
		#Update the current GPS position
		latitude.set(global_vars.thermalImages[global_vars.thermalCount-1].gps.latitude)
		longitude.set(global_vars.thermalImages[global_vars.thermalCount-1].gps.longitude)
	
	top.update_idletasks()
	top.update()
	