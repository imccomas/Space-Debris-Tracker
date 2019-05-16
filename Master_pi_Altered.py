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

#index = 1
# This is the address we setup in the Arduino Program
address = 0x10

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
    while True:
        if state:
            #start_time_bb1 = time.time() 
            data = cf.BB_1()
            #elapsed_time = time.time() - start_time_bb1
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
            while True:
                if state:
                    for i in range(len(data)):
                        print('C1 ', data[0])
                        print('C1 ', data[1])
                        print('C1 ', data[2])
                        bus.write_i2c_block_data(address,command,data[i])
                        time.sleep(.5)

                    #data.close()

                    #sleep one second
                    #index += 1
                    break
            break

def bb_2_telemetry():
    while True:
        
        if state:
            #start_time_bb2 = time.time() 
            data = cf.BB_2()
            #elapsed_time = time.time() - start_time_bb2
            #time.sleep(1)
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
            while True:
                if state:
                    for i in range(len(data)):
                        print('C1 ', data[0])
                        print('C1 ', data[1])
                        print('C1 ', data[2])
                        bus.write_i2c_block_data(address,command,data[i])
                        time.sleep(.5)

                    #data.close()
                    
                    #sleep one second
                    #index += 1
                    break
            break

def bb_3_telemetry():
    while True:
        
        if state:
            #start_time_bb3 = time.time() 
            data = cf.BB_3()
            #elapsed_time = time.time() - start_time_bb3
            #time.sleep(1)
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
            while True:
                if state:
                    for i in range(len(data)):
                        print('C1 ', data[0])
                        print('C1 ', data[1])
                        print('C1 ', data[2])
                        bus.write_i2c_block_data(address,command,data[i])
                        time.sleep(.5)

                    #data.close()

                    #sleep one second
                    #index += 1
                    break
            break
    
def bb_4_telemetry():
    while True:
        
        if state:
            #start_time_bb4 = time.time() 
            data = cf.BB_4()
            #elapsed_time = time.time() - start_time_bb4
            #time.sleep(1)
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
            while True:
                if state:
                    for i in range(len(data)):
                        print('C1 ', data[0])
                        print('C1 ', data[1])
                        print('C1 ', data[2])
                        bus.write_i2c_block_data(address,command,data[i])
                        time.sleep(.5)

                    #data.close()

                    #sleep one second
                    #index += 1
                    break
            break



bb_1_telemetry() 

bb_2_telemetry()

bb_3_telemetry()

bb_4_telemetry()
