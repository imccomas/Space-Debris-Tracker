from PIL import Image
import numpy as np

import time
import picamera

start_time = time.time()

position = []
elapsed_time = time.time() - start_time
with picamera.PiCamera() as camera:
        
    camera.resolution = (800,600)
    camera.start_preview()
    camera.start_recording("Video_Feed.h264")
    camera.wait_recording(1)
    camera.capture("backlog_image.jpg",use_video_port=True)

    background = Image.open("backlog_image.jpg")
    background = background.convert("L")
    background = background.resize((100,100), Image.ANTIALIAS)
    background = np.array(background,dtype=np.int16)
    

    while elapsed_time < 20:
        
        elapsed_time = time.time() - start_time
        
        camera.wait_recording(3)
        camera.capture("new_image.jpg", use_video_port=True)
        
        new_array = Image.open("new_image.jpg")
        new_array = new_array.convert("L")
        new_array = new_array.resize((100,100), Image.ANTIALIAS)
        new_array = np.array(new_array,dtype=np.int16)
        
        diff_matrix = new_array.__sub__(background)
        find = np.where(np.abs(diff_matrix)>10)
        location = (np.array(find).T)[0]
        pixels = np.shape(np.array(find).T)[0]
        x = np.mean(location[0])
        y = np.mean(location[1])
        position.append([x,y,pixels])
        
        elapsed_time = time.time() - start_time
        
    camera.stop_recording
