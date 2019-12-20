# -*- coding: utf-8 -*-
"""
Created on Wed Aug  7 12:19:06 2019

@author: imcco
"""

import numpy as np

text_file = open("rs232_output.txt", "r", errors = 'ignore')

lines = text_file.read().splitlines()
raw_array = []
for line in lines:
    raw_array.append(line.split('\n'))

raw_array = np.array(raw_array)
print (raw_array)

for i in range(len(raw_array)):
    











