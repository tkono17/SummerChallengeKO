#!/usr/bin/env python3
import serial
import time
import sys
import re
import argparse



print('open serial port')

def sendCommand(ser, data):
    ser.write( (data+'\r').encode())
    a=ser.read(100)
    return a

args = sys.argv
parser = argparse.ArgumentParser(
    prog=args[0],
    usage="Move Stage",
    description="HELP",
    epilog="END",
    add_help=True,
    )

parser.add_argument('-o', '--origin', help='back to the origin', action='store_true')
parser.add_argument('-r', '--reset', help='reset data', action='store_true')
parser.add_argument('-m', '--move', help='move by number of pulses', action='store_true')
parser.add_argument('-a', '--action', help='move to the cordinate of the pulse', action='store_true')
parser.add_argument('-s', '--stop', help='stop the action', action='store_true')
parser.add_argument('-q', '--q', help='return the data of stage', action='store_true')

#parser.add_argument('sign', help='sign', type=str, action='store_true')

ser = serial.Serial(port='/dev/ttyS0', baudrate=9600, timeout=1)


regex = r"%s"%args[1]

t = 0

sym = r"[-](o|r|m|a|l|q|h)"
check = re.compile(sym)
matchObj = check.match(regex)

if matchObj:
    t = t
else:
    t = t + 1
    print ("Error:No Such Command")

if t==0:
    args = parser.parse_args()
    if args.origin:
        regex2 = r"%s"%args[2]
        n = r"[+]|[-]"
        num = re.compile(n)
        matchNum = num.match(regex2)
        if matchNum:
            c1 = "H:"+"1"
            c2 = args[2]
            read = sendCommand(ser, c1) + sendCommand(ser, c2)
            print(c1+c2)
            print(read)
        else:
            print("Error: You should enter \' +/- \'")

    if args.reset:
        #print (matchObj.group())
        c1 = "R:1"
        read = sendCommand(ser, c1)
        print(c1)
        print(read)

    #matchObj = pattern.match("-m")
    if args.move:
        print (matchObj.group())
        regex2 = r"%s"%args[2]
        n = r"[+]P\d|[-]P\d"
        num = re.compile(n)
        matchNum = num.match(regex2)
        if matchNum:
            c1 = "M:"+'1'+args[2] 
            c2 = "G:"
            read = sendCommand(ser, c1) + sendCommand(ser, c2)
            print(c1+c2)
            print(read)
        else:
            print("Error: You should enter \'+/- P [Number]\'")

    #matchObj = pattern.match("-a")
    if args.action:
        print (matchObj.group())
        regex2 = r"%s"%args[2]
        n = r"[+]P\d|[-]P\d"
        num = re.compile(n)
        matchNum = num.match(regex2)
        if matchNum:
            c1 = "A:"+"1"+args[2]
            c2 = "G:"
            read = sendCommand(ser, c1) + sendCommand(ser, c2)
            print(c1+c2)
            print(read)
        else:
            print("Error: You should enter \'+/- P [Number]\'")

    #matchObj = pattern.match("-s")
    if args.stop:
        print (matchObj.group())
        c1 = "L:"
        read = sendCommand(ser, c1)
        print(c1)
        print(read)

    #matchObj = pattern.match("-q")
    if args.q:
        print (matchObj.group())
        c1 = "Q:"
        read = sendCommand(ser, c1)
        print(c1)
        print(read)

ser.close()
