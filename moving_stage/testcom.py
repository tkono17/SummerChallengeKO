#!/usr/bin/env python3
import serial
import time
import sys
import re

args = sys.argv

print('open serial port')

def sendCommand(ser, data):
    ser.write( (data+'\r').encode())
    a=ser.read(100)
    return a


ser = serial.Serial(port='/dev/ttyS0', baudrate=9600, timeout=1)
#print(ser.name)
#print(a)
#ser.write("Q:".encode())
#print('Write Q')
#a = sendCommand(ser, 'Q:')
#print(a)


regex = args[1]
pattern = re.compile(regex)

matchObj = pattern.match("M:")
if matchObj:
	print (matchObj.group())
	c = args[1]+'1'+args[2]	
	g = "G:"
	read = sendCommand(ser, c) + sendCommand(ser, g)
	print(c)
	print(read)

matchObj = pattern.match("Q:")



#regex = r"M:"
#pattern = re.compile(regex)
#text = "M:"
#matchObj = re.match(pattern, text)
#if matchObj:
#	print (matchObj.group())

sys.exit(0)

while False:
    c = input('Enter command>')
    if c == 'quit':
        break
    elif c == 'help':
	#print('  command, option, distance')
        print('Available commands:')
        print('  help  == ')
        print('  quit')
        print('  Q:')
        print('  A:')
        #pass
    else:
        a = sendCommand(ser, c)
        print('Command sent: %s' % c)
        print('  => %s' % a)
#sys.exit(0)

#ser.write("G:".encode())
#print('M:1+P1000'.encode())
#print('G:'.encode())
ser.close()
