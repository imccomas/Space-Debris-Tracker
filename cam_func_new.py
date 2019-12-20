from PIL import Image #image package
import numpy as np #numerical analysis package

import time #internal time keeping
import picamera #camera package for raspberry pi's

import os.path #package to keep track of files within memory
from os import path #same as above


from fractions import Fraction #ability to read ration numbers as fractions, specifically for framerate

def BB_1(): #the control launch for the experiment, this will be fired before any charging
    start_time_bb1 = time.time() #set start time

    position_bb1 = [] #array for positional values during experiment
    elapsed_time = time.time() - start_time_bb1 #set counter
    with picamera.PiCamera(framerate=Fraction('70/1')) as camera: #shortcut for camera call, also frame rate set (70 frames per second)
            
        camera.resolution = (640,480) #minimal camera resolution for higher frame rate
        camera.start_preview() #preview for testing ** comment out for actual flight cards **
        n=1 #number value for name place holder
        while True:
            if path.exists("Video_Feed_bb1_%d.h264" % n): #check if file already exists
                n += 1 #if it does add one
            else:
                break #if not move on

        camera.start_recording("Video_Feed_bb1_%d.h264" % n) #start the recording
        camera.shutter_speed = camera.exposure_speed
        camera.exposure_mode = 'off'
        camera.capture("backlog_image.jpg",use_video_port=True) #take the background image

        #Image break down proccess for library image

        background = Image.open("backlog_image_bb1.jpg") #open and save image to variable 
        background = background.convert("L") #grayscale the image (convert tensor to matrix)
        background = background.resize((100,100), Image.ANTIALIAS) #resize image for faster processing
        background = np.array(background,dtype=np.int16) #convert data type for later numerical analysis
        

        while elapsed_time < 6: #continue filming and data collection process for (8) seconds
            
            elapsed_time = time.time() - start_time_bb1 #keeping track of time
            
            camera.wait_recording(1) #delay image capture to every second
            camera.capture("new_image.jpg", use_video_port=True) #capture new image
            
            #Image break down proccess for experiment
            
            new_array = Image.open("new_image.jpg") #open and save image to variable
            new_array = new_array.convert("L") #grayscale image
            new_array = new_array.resize((100,100), Image.ANTIALIAS) #resize image
            new_array = np.array(new_array,dtype=np.int16) #convert date type
            
            diff_matrix = new_array.__sub__(background) #make a numerical comparison with background image (taking the difference between the two)
            find = np.where(np.abs(diff_matrix)>20) #find where the differences are within the matrix current threshhold is (10)

            if not np.any(find[0]): #if no movement is found (ie- nothing new pops up in the image)
                location = [0,0] #set location to 0
                pixels = 0 #set pixel value to 0
            else:
                location = (np.array(find).T)[0] #otherwise take the array of values where there is something new
                pixels = np.shape(np.array(find).T)[0] #and keep track of how many pixels are affected
            
            # Date packaging for neatness and ease of tranfer
            
            roll = int(pixels/255) #break pixels down to how many times 255 goes into the total
            remain = int(pixels%255) #and what the remainder is
            location = location.T #transpose the array location array 
            x = int(np.mean(location[0])) #calculate the average x
            y = int(np.mean(location[1])) #calculate the average y
            
            #append each value to the positional array
            
            position_bb1.append(x)
            position_bb1.append(y)
            position_bb1.append(roll)
            position_bb1.append(remain)
            
            elapsed_time = time.time() - start_time_bb1 #check time again
            
        camera.stop_recording #finish recording after designated experiment time
    return position_bb1 #return positional array to wherever called


def BB_2(): #fist experiment launch, second overall launch
    start_time_bb2 = time.time() #set start time

    position_bb2 = [] #positional array for second launch
    elapsed_time = time.time() - start_time_bb2 #set counter
    with picamera.PiCamera(framerate=Fraction('70/1')) as camera: #shortcut for camera call, also frame rate set (70 frames per second)
            
        camera.resolution = (640,480) #minimal camera resolution for higher frame rate
        camera.start_preview() #preview for testing ** comment out for actual flight cards **
        n=1 #number value for name place holder
        while True:
            if path.exists("Video_Feed_bb2_%d.h264" % n): #check if file already exists
                n += 1 #if it does add one
            else:
                break #if not move on

        camera.start_recording("Video_Feed_bb2_%d.h264" % n) #start the recording
        camera.shutter_speed = camera.exposure_speed
        camera.exposure_mode = 'off'
        camera.capture("backlog_image_bb2.jpg",use_video_port=True) #take the background image

        #Image break down proccess for library image

        background = Image.open("backlog_image.jpg") #open and save image to variable 
        background = background.convert("L") #grayscale the image (convert tensor to matrix)
        background = background.resize((100,100), Image.ANTIALIAS) #resize image for faster processing
        background = np.array(background,dtype=np.int16) #convert data type for later numerical analysis
        

        while elapsed_time < 8: #continue filming and data collection process for (8) seconds
            
            elapsed_time = time.time() - start_time_bb2 #keeping track of time
            
            camera.wait_recording(1) #delay image capture to every second
            camera.capture("new_image.jpg", use_video_port=True) #capture new image
            
            #Image break down proccess for experiment
            
            new_array = Image.open("new_image.jpg") #open and save image to variable
            new_array = new_array.convert("L") #grayscale image
            new_array = new_array.resize((100,100), Image.ANTIALIAS) #resize image
            new_array = np.array(new_array,dtype=np.int16) #convert date type
            
            diff_matrix = new_array.__sub__(background) #make a numerical comparison with background image (taking the difference between the two)
            find = np.where(np.abs(diff_matrix)>20) #find where the differences are within the matrix current threshhold is (10)

            if not np.any(find[0]): #if no movement is found (ie- nothing new pops up in the image)
                location = [0,0] #set location to 0
                pixels = 0 #set pixel value to 0
            else:
                location = (np.array(find).T)[0] #otherwise take the array of values where there is something new
                pixels = np.shape(np.array(find).T)[0] #and keep track of how many pixels are affected
            
            # Date packaging for neatness and ease of tranfer
            
            roll = int(pixels/255) #break pixels down to how many times 255 goes into the total
            remain = int(pixels%255) #and what the remainder is
            location = location.T #transpose the array location array 
            x = int(np.mean(location[0])) #calculate the average x
            y = int(np.mean(location[1])) #calculate the average y
            
            #append each value to the positional array
            
            position_bb2.append(x)
            position_bb2.append(y)
            position_bb2.append(roll)
            position_bb2.append(remain)
            
            elapsed_time = time.time() - start_time_bb2 #check time again
            
        camera.stop_recording #finish recording after designated experiment time
    return position_bb2 #return positional array to wherever called


def BB_3(): #second experimental launch, third launch overall
    start_time_bb3 = time.time() #set start time

    position_bb3 = [] #posoitional value for third launch
    elapsed_time = time.time() - start_time_bb3 #set counter
    with picamera.PiCamera(framerate=Fraction('70/1')) as camera: #shortcut for camera call, also frame rate set (70 frames per second)
            
        camera.resolution = (640,480) #minimal camera resolution for higher frame rate
        camera.start_preview() #preview for testing ** comment out for actual flight cards **
        n=1 #number value for name place holder
        while True:
            if path.exists("Video_Feed_bb3_%d.h264" % n): #check if file already exists
                n += 1 #if it does add one
            else:
                break #if not move on

        camera.start_recording("Video_Feed_bb3_%d.h264" % n) #start the recording
        camera.shutter_speed = camera.exposure_speed
        camera.exposure_mode = 'off'
        camera.capture("backlog_image.jpg",use_video_port=True) #take the background image

        #Image break down proccess for library image

        background = Image.open("backlog_image_bb3.jpg") #open and save image to variable 
        background = background.convert("L") #grayscale the image (convert tensor to matrix)
        background = background.resize((100,100), Image.ANTIALIAS) #resize image for faster processing
        background = np.array(background,dtype=np.int16) #convert data type for later numerical analysis
        

        while elapsed_time < 8: #continue filming and data collection process for (8) seconds
            
            elapsed_time = time.time() - start_time_bb3 #keeping track of time
            
            camera.wait_recording(1) #delay image capture to every second
            camera.capture("new_image.jpg", use_video_port=True) #capture new image
            
            #Image break down proccess for experiment
            
            new_array = Image.open("new_image.jpg") #open and save image to variable
            new_array = new_array.convert("L") #grayscale image
            new_array = new_array.resize((100,100), Image.ANTIALIAS) #resize image
            new_array = np.array(new_array,dtype=np.int16) #convert date type
            
            diff_matrix = new_array.__sub__(background) #make a numerical comparison with background image (taking the difference between the two)
            find = np.where(np.abs(diff_matrix)>20) #find where the differences are within the matrix current threshhold is (10)

            if not np.any(find[0]): #if no movement is found (ie- nothing new pops up in the image)
                location = [0,0] #set location to 0
                pixels = 0 #set pixel value to 0
            else:
                location = (np.array(find).T)[0] #otherwise take the array of values where there is something new
                pixels = np.shape(np.array(find).T)[0] #and keep track of how many pixels are affected
            
            # Date packaging for neatness and ease of tranfer
            
            roll = int(pixels/255) #break pixels down to how many times 255 goes into the total
            remain = int(pixels%255) #and what the remainder is
            location = location.T #transpose the array location array 
            x = int(np.mean(location[0])) #calculate the average x
            y = int(np.mean(location[1])) #calculate the average y
            
            #append each value to the positional array
            
            position_bb3.append(x)
            position_bb3.append(y)
            position_bb3.append(roll)
            position_bb3.append(remain)
            
            elapsed_time = time.time() - start_time_bb3 #check time again
            
        camera.stop_recording #finish recording after designated experiment time
    return position_bb3 #return positional array to wherever called


def BB_4(): #third experimental launch, fourth overall 
    start_time_bb4 = time.time() #set start time

    position_bb4 = [] #positional array for fourth launch
    elapsed_time = time.time() - start_time_bb4 #set counter
    with picamera.PiCamera(framerate=Fraction('70/1')) as camera: #shortcut for camera call, also frame rate set (70 frames per second)
            
        camera.resolution = (640,480) #minimal camera resolution for higher frame rate
        camera.start_preview() #preview for testing ** comment out for actual flight cards **
        n=1 #number value for name place holder
        while True:
            if path.exists("Video_Feed_bb4_%d.h264" % n): #check if file already exists
                n += 1 #if it does add one
            else:
                break #if not move on

        camera.start_recording("Video_Feed_bb4_%d.h264" % n) #start the recording
        camera.shutter_speed = camera.exposure_speed
        camera.exposure_mode = 'off'
        camera.capture("backlog_image.jpg",use_video_port=True) #take the background image

        #Image break down proccess for library image

        background = Image.open("backlog_image_bb4.jpg") #open and save image to variable 
        background = background.convert("L") #grayscale the image (convert tensor to matrix)
        background = background.resize((100,100), Image.ANTIALIAS) #resize image for faster processing
        background = np.array(background,dtype=np.int16) #convert data type for later numerical analysis
        

        while elapsed_time < 8: #continue filming and data collection process for (8) seconds
            
            elapsed_time = time.time() - start_time_bb4 #keeping track of time
            
            camera.wait_recording(1) #delay image capture to every second
            camera.capture("new_image.jpg", use_video_port=True) #capture new image
            
            #Image break down proccess for experiment
            
            new_array = Image.open("new_image.jpg") #open and save image to variable
            new_array = new_array.convert("L") #grayscale image
            new_array = new_array.resize((100,100), Image.ANTIALIAS) #resize image
            new_array = np.array(new_array,dtype=np.int16) #convert date type
            
            diff_matrix = new_array.__sub__(background) #make a numerical comparison with background image (taking the difference between the two)
            find = np.where(np.abs(diff_matrix)>20) #find where the differences are within the matrix current threshhold is (10)

            if not np.any(find[0]): #if no movement is found (ie- nothing new pops up in the image)
                location = [0,0] #set location to 0
                pixels = 0 #set pixel value to 0
            else:
                location = (np.array(find).T)[0] #otherwise take the array of values where there is something new
                pixels = np.shape(np.array(find).T)[0] #and keep track of how many pixels are affected
            
            # Date packaging for neatness and ease of tranfer
            
            roll = int(pixels/255) #break pixels down to how many times 255 goes into the total
            remain = int(pixels%255) #and what the remainder is
            location = location.T #transpose the array location array 
            x = int(np.mean(location[0])) #calculate the average x
            y = int(np.mean(location[1])) #calculate the average y
            
            #append each value to the positional array
            
            position_bb4.append(x)
            position_bb4.append(y)
            position_bb4.append(roll)
            position_bb4.append(remain)
            
            elapsed_time = time.time() - start_time_bb4 #check time again
            
        camera.stop_recording #finish recording after designated experiment time
    return position_bb4 #return positional array to wherever called

