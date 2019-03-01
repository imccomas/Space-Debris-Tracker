# -*- coding: utf-8 -*-
"""
Created on Thu Jan 24 10:19:01 2019

@author: imcco
"""

import matplotlib.pyplot as plt
from PIL import Image
import numpy as np
import matplotlib.image as mpimg


def make_a_point(x,y): #x and y are for 
    

    
grey = Image.open("White Background Image.png")   
grey = grey.convert("L")
grey = grey.resize((100,100),Image.ANTIALIAS)
grey = np.array(grey)
print (grey)


print (grey[50,50])
grey[50,50] = 150
print (grey[50,50])

plt.imshow(grey)
plt.show()














