# -*- coding: utf-8 -*-
"""
Created on Mon Jan 28 16:15:05 2019

@author: imcco
"""

from datetime import datetime
import picamera


camera = picamera.PiCamera()
camera.capture("backlog_image.jpg")
camera.start_recording("important_information_dont_delete.h264")
#start_time = datetime.now()
#time_elapesed.seconds is the function to call for the internal clock of the pi zero

"""
Within this loop we need to run the image recongnition process, this is why we CAN'T use time(sleep) function...
... because it won't allow other processes to happen.

one of the main goals is; can we pull images from the camera file while we are recording. this could be a hardware...
... issue
"""

camera.stop_recording()















