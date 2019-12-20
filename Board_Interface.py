
#RockSat-X 2019 Main i2c Bus
#Written by James Cook
#Communication Between Arduino and RasPi
#Spring 2019 

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
import RPi.GPIO as GPIO #library that allows access to GPIO pins on the raspberry pi's 
import smbus #Information bus library, used in this case specifically for the I2C aspects
import time #keeping time
import numpy as np #numerical analysis
import cam_func_update as cf #importing the camera function library
# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1) #designating number of modules on bus
GPIO.setmode(GPIO.BOARD) #sets pin to sense when to transmit
GPIO.setup(38, GPIO.IN) #designating signal pin from arduino for data transfer readiness
state = GPIO.input(38) #variable for GPIO input for testing
#index = 1
# This is the address we setup in the Arduino Program
address = 0x10 #hex address available for I2C


def writeNumber(data): # changed value to data JR
    #Bypassing function, JR
    #bus.write_byte(address, data) #Same JR
    #bus.write_word_data(address, info)
    #bus.read_i2c_block_data(address, lines, dtype = np.int(16))
    #data.close()
    bus.read_i2c_block_data(address, lines, dtype = np.int(16))
    data.close()
    return -1

def readNumber():
    number = bus.read_byte(address)
    # number = bus.read_byte_data(address, 1)
    return number


def bb_1_telemetry(): #control launch, first launch overall
    GPIO.add_event_detect(38, GPIO.RISING) #wait for signal from arduino
    while True: #waiting loop

        if GPIO.event_detected(38): #if detection occurs
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38) #remove event (reset detection)
            GPIO.add_event_detect(38, GPIO.RISING) #add new event (still reseting)
            data = cf.BB_1() #call camera for bb1/control launch
            #elapsed_time = time.time() - start_time_bb1
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00 #comand to let arduino know data type being sent over line
            #test = [1,2,3,4]
            while True: #wait for next signal
                if GPIO.event_detected(38): #when second signal is detected 
                    GPIO.remove_event_detect(38) #remove event (full reset)
                    bus.write_i2c_block_data(address,command,data) #send block of data through I2C using specified address
            
   

                    #data.close()

                    #sleep one second
                    #index += 1
                    break #break second loop
            break #break first loop

def bb_2_telemetry(): #first experimental launch, second launch overall
    GPIO.add_event_detect(38, GPIO.RISING) #wait for signal from arduino
    while True: #waiting loop

        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
            #start_time_bb2 = time.time()
            GPIO.add_event_detect(38, GPIO.RISING)
            data = cf.BB_2()
            #elapsed_time = time.time() - start_time_bb2
            #time.sleep(1)
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
            GPIO.add_event_detect(38, GPIO.RISING)
            while True:
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    bus.write_i2c_block_data(address,command,data)

                    #data.close()
                    
                    #sleep one second
                    #index += 1
                    break
            break

def bb_3_telemetry():
    GPIO.add_event_detect(38, GPIO.RISING)
    while True:

        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
            #start_time_bb3 = time.time()
            GPIO.add_event_detect(38, GPIO.RISING)
            data = cf.BB_3()
            #elapsed_time = time.time() - start_time_bb3
            #time.sleep(1)
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
            GPIO.add_event_detect(38, GPIO.RISING)
            while True:
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    bus.write_i2c_block_data(address,command,data)

                    #data.close()

                    #sleep one second
                    #index += 1
                    break
            break
    
def bb_4_telemetry():
    GPIO.add_event_detect(38, GPIO.RISING)
    while True:
        #print ("waiting")
        if GPIO.event_detected(38):
            #start_time_bb1 = time.time()
            GPIO.remove_event_detect(38)
            #start_time_bb4 = time.time()
            GPIO.add_event_detect(38, GPIO.RISING)
            data = cf.BB_4()
            #elapsed_time = time.time() - start_time_bb4
            #time.sleep(1)
            #data = open("Hi.txt", "r")
            #line = np.array(data)
            #lines = data.readlines()
            #lines = np.array(data, dtype = np.int16)
            command = 0x00
            GPIO.add_event_detect(38, GPIO.RISING)
            while True:
                if GPIO.event_detected(38):
                    GPIO.remove_event_detect(38)
                    bus.write_i2c_block_data(address,command,data)

                    #data.close()

                    #sleep one second
                    #index += 1
                    break
            break

def shutdown():
    from subprocess import call
    call("sudo shutdown -h now", shell=True)


bb_1_telemetry() 

bb_2_telemetry()

bb_3_telemetry()

bb_4_telemetry()

shutdown()
