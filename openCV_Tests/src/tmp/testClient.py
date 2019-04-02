#!/usr/bin/env python
    
import socket 
import numpy as np
import pickle

TCP_IP = '10.0.02'
TCP_PORT = 10000
BUFFER_SIZE = 8192
MESSAGE = "Hello, World!"

#singleLine = "0x00000010203040506070809101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960"
test = "0x00"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
print "connected!"
s.send(test)
data = s.recv(BUFFER_SIZE)
s.close() 
print "received data:", data 
