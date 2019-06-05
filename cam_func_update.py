from PIL import Image
import numpy as np

import time
import picamera


def BB_1(): 
    start_time_bb1 = time.time()

    position_bb1 = []
    elapsed_time = time.time() - start_time_bb1
    with picamera.PiCamera() as camera:
            
        camera.resolution = (800,600)
        camera.start_preview()
        n=1
        while True:
            if path.exists("Video_Feed_bb1_%d.h264" % n):
                n+=1
            else:
                break
        camera.start_recording("Video_Feed_bb1_%d.h264")
        camera.wait_recording(1)
        camera.capture("backlog_image.jpg",use_video_port=True)

        background = Image.open("backlog_image.jpg")
        background = background.convert("L")
        background = background.resize((100,100), Image.ANTIALIAS)
        background = np.array(background,dtype=np.int16)
        

        while elapsed_time < 9:
            
            elapsed_time = time.time() - start_time_bb1
            
            camera.wait_recording(1)
            camera.capture("new_image.jpg", use_video_port=True)
            
            new_array = Image.open("new_image.jpg")
            new_array = new_array.convert("L")
            new_array = new_array.resize((100,100), Image.ANTIALIAS)
            new_array = np.array(new_array,dtype=np.int16)
            
            diff_matrix = new_array.__sub__(background)
            find = np.where(np.abs(diff_matrix)>10)
            if not np.any(find[0]):
                location = [0,0]
                pixels = 0
            else:
                location = (np.array(find).T)[0]
                pixels = np.shape(np.array(find).T)[0]
            
            roll = int(pixels/255)
            remain = int(pixels%255)
            x = int(np.mean(location[0] + 1))
            y = int(np.mean(location[1] + 1))
            position_bb1.append([x,y,roll,remain])
            
            elapsed_time = time.time() - start_time_bb1
            
        camera.stop_recording
    return position_bb1


def BB_2():
    start_time_bb2 = time.time()

    position_bb2 = []
    elapsed_time = time.time() - start_time_bb2
    with picamera.PiCamera() as camera:
            
        camera.resolution = (800,600)
        camera.start_preview()
        n=1
        while True:
            if path.exists("Video_Feed_bb2_%d.h264" % n):
                n+=1
            else:
                break
        camera.start_recording("Video_Feed_bb2_%d.h264" % n)
        camera.wait_recording(1)
        camera.capture("backlog_image.jpg",use_video_port=True)

        background = Image.open("backlog_image.jpg")
        background = background.convert("L")
        background = background.resize((100,100), Image.ANTIALIAS)
        background = np.array(background,dtype=np.int16)
        

        while elapsed_time < 9:
            
            elapsed_time = time.time() - start_time_bb2
            
            camera.wait_recording(3)
            camera.capture("new_image.jpg", use_video_port=True)
            
            new_array = Image.open("new_image.jpg")
            new_array = new_array.convert("L")
            new_array = new_array.resize((100,100), Image.ANTIALIAS)
            new_array = np.array(new_array,dtype=np.int16)
            
            diff_matrix = new_array.__sub__(background)
            find = np.where(np.abs(diff_matrix)>10)
            if not np.any(find[0]):
                location = [0,0]
                pixels = 0
            else:
                location = (np.array(find).T)[0]
                pixels = np.shape(np.array(find).T)[0]
            
            roll = int(pixels/255)
            remain = int(pixels%255)
            x = int(np.mean(location[0] + 1))
            y = int(np.mean(location[1] + 1))
            position_bb2.append([x,y,roll,remain])
            
            elapsed_time = time.time() - start_time_bb2
            
        camera.stop_recording
    return position_bb2


def BB_3():
    start_time_bb3 = time.time()

    position_bb3 = []
    elapsed_time = time.time() - start_time_bb3
    with picamera.PiCamera() as camera:
            
        camera.resolution = (800,600)
        camera.start_preview()
        n=1
        while True:
            if path.exists("Video_Feed_bb3_%d.h264" % n):
                n+=1
            else:
                break
        camera.start_recording("Video_Feed_bb3_%d.h264" % n)
        camera.wait_recording(1)
        camera.capture("backlog_image.jpg",use_video_port=True)

        background = Image.open("backlog_image.jpg")
        background = background.convert("L")
        background = background.resize((100,100), Image.ANTIALIAS)
        background = np.array(background,dtype=np.int16)
        

        while elapsed_time < 9:
            
            elapsed_time = time.time() - start_time_bb3
            
            camera.wait_recording(3)
            camera.capture("new_image.jpg", use_video_port=True)
            
            new_array = Image.open("new_image.jpg")
            new_array = new_array.convert("L")
            new_array = new_array.resize((100,100), Image.ANTIALIAS)
            new_array = np.array(new_array,dtype=np.int16)
            
            diff_matrix = new_array.__sub__(background)
            find = np.where(np.abs(diff_matrix)>10)
            if not np.any(find[0]):
                location = [0,0]
                pixels = 0
            else:
                location = (np.array(find).T)[0]
                pixels = np.shape(np.array(find).T)[0]
            
            roll = int(pixels/255)
            remain = int(pixels%255)
            x = int(np.mean(location[0] + 1))
            y = int(np.mean(location[1] + 1))
            position_bb3.append([x,y,roll,remain])
            
            elapsed_time = time.time() - start_time_bb3
            
        camera.stop_recording
    return position_bb3


def BB_4():
    start_time_bb4 = time.time()

    position_bb4 = []
    elapsed_time = time.time() - start_time_bb4
    with picamera.PiCamera() as camera:
            
        camera.resolution = (800,600)
        camera.start_preview()
        n=1
        while True:
            if path.exists("Video_Feed_bb4_%d.h264" % n):
                n+=1
            else:
                break
        camera.start_recording("Video_Feed_bb4_%d.h264" % n)
        camera.wait_recording(1)
        camera.capture("backlog_image.jpg",use_video_port=True)

        background = Image.open("backlog_image.jpg")
        background = background.convert("L")
        background = background.resize((100,100), Image.ANTIALIAS)
        background = np.array(background,dtype=np.int16)
        

        while elapsed_time < 9:
            
            elapsed_time = time.time() - start_time_bb4
            
            camera.wait_recording(3)
            camera.capture("new_image.jpg", use_video_port=True)
            
            new_array = Image.open("new_image.jpg")
            new_array = new_array.convert("L")
            new_array = new_array.resize((100,100), Image.ANTIALIAS)
            new_array = np.array(new_array,dtype=np.int16)
            
            diff_matrix = new_array.__sub__(background)
            find = np.where(np.abs(diff_matrix)>10)
            if not np.any(find[0]):
                location = [0,0]
                pixels = 0
            else:
                location = (np.array(find).T)[0]
                pixels = np.shape(np.array(find).T)[0]
            
            roll = int(pixels/255)
            remain = int(pixels%255)
            x = int(np.mean(location[0] + 1))
            y = int(np.mean(location[1] + 1))
            position_bb4.append([x,y,roll,remain])
            
            elapsed_time = time.time() - start_time_bb4
            
        camera.stop_recording
    return position_bb4

