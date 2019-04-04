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
import glob
import os
import main

def buttonClick():
	main.thermalMission()

def visualButton():
	main.visualMission()

top = tk.Tk()
top.geometry('1000x1000')
top.title('Dronez')
#Widgets

#left frame
leftFrame = tk.Frame(top, height = 900, width = 100, bg = "red")
leftFrame.pack(side = tk.LEFT)

#left top frame
#******************

leftTop = tk.Frame(leftFrame, height = 300, width = 200, bg = "blue")
leftTop.pack(fill=tk.X)

leftMiddle = tk.Frame(leftFrame, height = 300, width = 200, bg = "brown")
leftMiddle.pack(fill=tk.X)

leftBottom = tk.Frame(leftFrame, height = 300, width = 200, bg = "black")
leftBottom.pack(fill=tk.X)

#start/stop button
button = tk.Button(leftTop, command=buttonClick, text = "Start Thermal")
button.pack()

button2 = tk.Button(leftTop, command=visualButton, text = "Start Visual")
button2.pack()
#mission stats field

#right frame
#*******************
rightFrame = tk.Frame(top, height = 900, width = 700)
rightFrame.pack(side = tk.RIGHT)

#Image frame - top right
rightTop = tk.Frame(rightFrame, height = 450, bg = "black")
rightTop.pack(side = tk.TOP, fill=tk.BOTH)

#Map frame - bottom right
rightBottom = tk.Frame(rightFrame, height = 450, bg = "black")
rightBottom.pack(side = tk.BOTTOM, fill=tk.BOTH)

#Thermal Image canvas
imgCanvas = tk.Canvas(rightTop)
imgCanvas.pack(fill=tk.BOTH)


#Open the newest image in the folder
#list_of_files = glob.glob("/home/ztumbleson/droneTeam/openCV_Tests/images/")
#newImage = max(list_of_files, key=os.path.getctime)
#newImage = "Thermal.jpg"
#img = ImageTk.PhotoImage(Image.open("/home/ztumbleson/droneTeam/openCV_Tests/images/" + newImage))
#image = imgCanvas.create_image(0, 0, image=img)

#Map canvas
mapCanvas = tk.Canvas(rightTop)
mapCanvas.pack(fill=tk.BOTH)
#mapImg = ImageTk.PhotoImage(Image.open("/home/ztumbleson/droneTeam/openCV_Tests/images/Thermal.jpg"))
#mapBox = mapCanvas.create_image(0, 0, image=mapImg)

top.mainloop()