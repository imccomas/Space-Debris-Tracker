import RPi.GPIO as GPIO
import smbus
import time
import numpy as np
# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1)
GPIO.setmode(GPIO.BOARD) #sets pin to sense when to transmit
GPIO.setup(38, GPIO.IN)

state = GPIO.input(38)

# This is the address we setup in the Arduino Program
address = 0x04

def writeNumber(data): # changed value to data JR
    #Bypassing function, JR
    #bus.write_byte(address, data) #Same JR
    #bus.write_word_data(address, info)
    bus.read_i2c_block_data(address, lines, dtype = np.int(16))
    data.close()
    return -1

def readNumber():
    number = bus.read_byte(address)
    # number = bus.read_byte_data(address, 1)
    return number

while True and state:
    
    #if state:
    data = open("Hi.txt" , "r")
    lines = data.readlines()
    lines = np.array(lines, dtype = np.int16)
    for line in lines:
        x = line
    print(x)
    bus.read_i2c_block_data(address, x, np.int(16))
    print ("RPI: Hi Arduino, I sent you a file")
    data.close()
    
        
    # sleep one second
    time.sleep(1)

    number = readNumber()
    print ("Arduino: Hey RPI, I received a digit ", number)
    #print
