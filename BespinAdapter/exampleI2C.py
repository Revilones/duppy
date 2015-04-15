#!/usr/bin/python

import smbus
import time
import sys
import struct

#Constants
SPACER = '*************************'
address = 0x04
offset = 2006
ohmPerMM = -5.9

#Create global object
bus = smbus.SMBus(1)

def writeNumber(value):

    bus.write_byte(address, value)

    return -1 

def readNumber():

    number = bus.read_i2c_block_data(address, 0x1, 4)

    return number

while True:
    var = 10
    number = 0

    #Wait a bit between samples
    time.sleep(1)

    try:
        number = readNumber()
    except IOError as e:
        print 'Cannot Read from Water Level Sensor.\n' \
              'I/O Error({0}): {1}\n' \
              'Waiting 10 Seconds for reconnect'.format(e.errno, e.strerror)
        print SPACER
        time.sleep(10)
    except:
        print 'Unexpected Error:', sys.exc_info()[0]
        print SPACER

    if number:
	print number
	tmp = "".join(map(chr,number))
	print tmp
	Vo = struct.unpack('f', tmp)[0]
	#Vo = struct.unpack('<f', struct.pack('4b', *number))[0]
        #Vo = float(number[3] << 24 | number[2] << 16 | number[1] << 8 | number[0])
	#waterHeight = -((Vo - offset) / float(ohmPerMM)) + 1;
	timeStamp = time.strftime('%X %x %Z')

	#tmp = '{0},{1}\n'.format(timeStamp,waterHeight)
	#f = open('tankLevel.csv', 'a')
	#f.write(tmp)
	#f.close()

	print ohmPerMM
	print timeStamp
        print "Vo", Vo 
	#print "Water Height {0}mm".format(waterHeight)
        print SPACER

