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

	TCP_IP = '10.0.0.2'
	TCP_PORT = 500
	BUFFER_SIZE = 164

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((TCP_IP, TCP_PORT))

	global_vars.stopThermal = False
	hotspot = False
	while(global_vars.stopThermal == False):
		top.update_idletasks()
		top.update()

		if(global_vars.invalidData == True):
			missions.flushSocket(s)
			s.recv(2)

		path = missions.thermalMission(s, BUFFER_SIZE)

		print "Path: " + path

		#hotspot, processedImage = improc.
		#Hotspot detected, update GUI and counter
		#if(hotspot):
		#	global_vars.hotspotCount = global_vars.hotspotCount + 1

			#TODO ADD IMAGE UPDATE CODE
		rawImage = Image.open(path)	

		resizedImage = rawImage.resize((640, 480), Image.NEAREST)

		displayReadyImage = ImageTk.PhotoImage(resizedImage)

		image = imgCanvas.create_image(400, 250, image=displayReadyImage)

			#Save image in hotspot directory
		#	image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
		#	new_name = global_vars.hotspotPath + "thermal" + str(global_vars.thermalCount)
		#	cv2.imwrite(new_name, image)

			#Update canvas
			#img = ImageTk.PhotoImage(Image.open(filename))
			#imgCanvas.create_image(0,0, image=img)

			#TODO ADD STATS UPDATE CODE
		global_vars.hotspotCount = global_vars.hotspotCount + 1

			#TODO ADD LIST UPDATE CODE


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

		resizedImage = rawImage.resize((780,480), Image.NEAREST)

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
# Contains Mission Stats and Exit button
#
#*****************************************

leftMiddle = tk.Frame(leftFrame, height = 300, width = 200, bg = "gray", borderwidth = 5, relief = tk.SUNKEN)
leftMiddle.pack_propagate(0)
leftMiddle.pack(fill=tk.BOTH)

#TODO: ADD Mission Stats

var = tk.StringVar()
timer = tk.StringVar()
statsLabel = tk.Label(leftMiddle, bg = "gray", \
	textvariable = var)

timerLabel = tk.Label(leftMiddle, textvariable = timer)
var.set("Mission Stats")
statsLabel.pack()
timerLabel.pack()


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

image = imgCanvas.create_image(400, 250, image=displayReadyImage)

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

while global_vars.fullStop == False:

	global_vars.missioncurrentTime = time.time()
	global_vars.timerTicks = global_vars.timerTicks + 1
	if(global_vars.timerTicks > 1000):
		timer.set(global_vars.missioncurrentTime - global_vars.missionStartTime)

	top.update_idletasks()
	top.update()
	