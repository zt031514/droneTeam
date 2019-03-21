#!/usr/bin/env python
    
import socket 
import numpy as np
import pickle

TCP_IP = '10.0.0.2'
TCP_PORT = 500
BUFFER_SIZE = 2048
MESSAGE = "Hello, World!"

#singleLine = "0x00000010203040506070809101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960"
test1 = "Start"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
print "connected!"
s.send(test1)
for x in range(60):
	data = s.recv(BUFFER_SIZE)
	print x
else:
	print "Done!"	
s.close()
