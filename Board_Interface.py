<<<<<<< HEAD
=======
#RockSat-X 2019 Main i2c Bus
#Written by James Cook
#Communication Between Arduino and RasPi
#Spring 2019 

>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912
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
import cam_func_update as cf
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
<<<<<<< HEAD
    #bus.read_i2c_block_data(address, lines, dtype = np.int(16))
    #data.close()
=======
    bus.read_i2c_block_data(address, lines, dtype = np.int(16))
    data.close()
>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912
    return -1

def readNumber():
    number = bus.read_byte(address)
    # number = bus.read_byte_data(address, 1)
    return number


def bb_1_telemetry():
    GPIO.add_event_detect(38, GPIO.RISING)
    while True:
<<<<<<< HEAD
        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
=======
        #print ("waiting")
        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
            GPIO.add_event_detect(38, GPIO.RISING)
>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912
            data = cf.BB_1()
            #elapsed_time = time.time() - start_time_bb1
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
            #test = [1,2,3,4]
<<<<<<< HEAD
            GPIO.add_event_detect(38, GPIO.RISING)
            while True:
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    bus.write_i2c_block_data(address,command,data)
=======
            
            while True:
                #print ("waiting")
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    for i in range(len(data)):
                        for j in range(len(data[i])):
                            print('C1 ', data[0])
                            print('C1 ', data[1])
                            print('C1 ', data[2])
                            bus.write_i2c_block_data(address,command,data[i])
   
>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912

                    #data.close()

                    #sleep one second
                    #index += 1
                    break
            break

def bb_2_telemetry():
    GPIO.add_event_detect(38, GPIO.RISING)
    while True:
<<<<<<< HEAD
        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
            #start_time_bb2 = time.time() 
=======
        #print ("waiting")
        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
            #start_time_bb2 = time.time()
            GPIO.add_event_detect(38, GPIO.RISING)
>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912
            data = cf.BB_2()
            #elapsed_time = time.time() - start_time_bb2
            #time.sleep(1)
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
<<<<<<< HEAD
            GPIO.add_event_detect(38, GPIO.RISING)
            while True:
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    bus.write_i2c_block_data(address,command,data)
=======
 
            while True:
                #print ("waiting")
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    for i in range(len(data)):
                        print('C1 ', data[0])
                        print('C1 ', data[1])
                        print('C1 ', data[2])
                        bus.write_i2c_block_data(address,command,data[i])
            
>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912

                    #data.close()
                    
                    #sleep one second
                    #index += 1
                    break
            break

def bb_3_telemetry():
    GPIO.add_event_detect(38, GPIO.RISING)
    while True:
<<<<<<< HEAD
        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
            #start_time_bb3 = time.time() 
=======
        #print ("waiting")
        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
            #start_time_bb3 = time.time()
            GPIO.add_event_detect(38, GPIO.RISING)
>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912
            data = cf.BB_3()
            #elapsed_time = time.time() - start_time_bb3
            #time.sleep(1)
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
<<<<<<< HEAD
            GPIO.add_event_detect(38, GPIO.RISING)
            while True:
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    bus.write_i2c_block_data(address,command,data)
=======
            
            while True:
                #print ("waiting")
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    for i in range(len(data)):
                        print('C1 ', data[0])
                        print('C1 ', data[1])
                        print('C1 ', data[2])
                        bus.write_i2c_block_data(address,command,data[i])
                      
>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912

                    #data.close()

                    #sleep one second
                    #index += 1
                    break
            break
    
def bb_4_telemetry():
    GPIO.add_event_detect(38, GPIO.RISING)
    while True:
<<<<<<< HEAD
        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
            #start_time_bb4 = time.time() 
=======
        #print ("waiting")
        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
            #start_time_bb4 = time.time()
            GPIO.add_event_detect(38, GPIO.RISING)
>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912
            data = cf.BB_4()
            #elapsed_time = time.time() - start_time_bb4
            #time.sleep(1)
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
<<<<<<< HEAD
            GPIO.add_event_detect(38, GPIO.RISING)
            while True:
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    bus.write_i2c_block_data(address,command,data)
=======

            while True:
                #print ("waiting")
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    for i in range(len(data)):
                        print('C1 ', data[0])
                        print('C1 ', data[1])
                        print('C1 ', data[2])
                        bus.write_i2c_block_data(address,command,data[i])
                        
>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912

                    #data.close()

                    #sleep one second
                    #index += 1
                    break
            break

def shutdown():
    from subprocess import call
    call("sudo shutdown -h now", shell=True)

<<<<<<< HEAD
=======

>>>>>>> cf32e1a81b161028736a3bd4ebb677591bd4b912
bb_1_telemetry() 

bb_2_telemetry()

bb_3_telemetry()

bb_4_telemetry()

shutdown()
