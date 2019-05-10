"""
###TO DO LIST PLANNING###
create a detailed timeline of events for the systems
save the timeline somewhere so we dont forget!

###TO DO LIST BUS###
integrate time functions into each telemtry function,
**look into the hotline?**
hookup different pi, to run camera
test camera functions, make sure they all work seperately
test all the timing

###TO DO LIST ARMS###
in between each data set, run arms code
set all arms to relative zeros
**check the servo system check, i.e. moving before called**

"""
import RPi.GPIO as GPIO
import smbus
import time
import numpy as np
import cam_func as cf
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


def bb_1_telemetry():
    while True and state:
        
        #if state:
        data = cf.BB_1()
        elapsed_time = time.time() - start_time_bb1
        time.sleep(1)
        #data = open("Hi.txt", "r")
        #line = np.array(data)
        #lines = data.readlines()
        lines = np.array(data, dtype = np.int16)

        if elapsed_time >= 10:
            for line in lines:
                x = line
                print(x[0])
                print(x[1])
                print(x[2])
                bus.read_i2c_block_data(address, x[0])
                bus.read_i2c_block_data(address, x[1])
                bus.read_i2c_block_data(address, x[2])
                #print ("RPI: Hi Arduino, I sent you a file")
            #data.close()

            
            # sleep one second
            time.sleep(2)
            break

def bb_2_telemetry():
    while True and state:
        
        #if state:
        #data = cf.BB_1
        data = open("Hi.txt", "r")
        lines = data.readlines()
        lines = np.array(lines, dtype = np.int16)

        for line in lines:
            x = line
            print(x)
            bus.read_i2c_block_data(address, x)
            #print ("RPI: Hi Arduino, I sent you a file")
        data.close()
        break
            
        # sleep one second
        time.sleep(2)

def bb_3_telemetry():
    while True and state:
        
        #if state:
        #data = cf.BB_1
        data = open("Hi.txt", "r")
        lines = data.readlines()
        lines = np.array(lines, dtype = np.int16)
        for line in lines:
            x = line
            print(x)
            bus.read_i2c_block_data(address, x)
            #print ("RPI: Hi Arduino, I sent you a file")
        data.close()

            
        # sleep one second
        time.sleep(2)
        break
    
def bb_4_telemetry():
    while True and state:
        
        #if state:
        #data = cf.BB_1
        data = open("Hi.txt", "r")
        lines = data.readlines()
        lines = np.array(lines, dtype = np.int16)
        for line in lines:
            x = line
            print(x)
            bus.read_i2c_block_data(address, x)
            #print ("RPI: Hi Arduino, I sent you a file")
        data.close()

            
        # sleep one second
        time.sleep(2)
        break

time.sleep(5)

start_time_bb1 = time.time()   
bb_1_telemetry()

bb_2_telemetry()
time.sleep(1)

bb_3_telemetry()
time.sleep(1)

bb_4_telemetry()
time.sleep(1)
