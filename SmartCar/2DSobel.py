#/usr/bin/python
# -*- coding: utf-8 -*-

import Tkinter as tk
import serial
import copy
from math import *

COL = 90
ROW = 60
BLACK = 0
WHITE = 255
count = 0
VALUE = 6
#################################################################################

def GetImage():
  global count
  global image
  
  f = open("c%d.txt" % count,'r')
  i = 0
  tmp = []
  tmp2 = []
  for eachLine in f:
    eachLine = eachLine.strip()
    tmp = eachLine.split(" ")
    for j,item in enumerate(tmp):
      image[i][j] = int(item)
    i = i + 1
  CalcSobel()
  root.event_generate('<<update>>',when = 'tail')
  
def PredImage():
  global count
  count = count - 1
  print count
  GetImage()
def SuccImage():
  global count
  count = count + 1
  print count
  GetImage()

pre = tk.Button(root,text = u'上一幅',command = PredImage)
suc = tk.Button(root,text = u'下一幅',command = SuccImage)
pre.pack()
suc.pack()

##########################################################################################

def ChangeColour(event):
  global count
  #for i in xrange(0,ROW,1):
  #  for j in xrange(0,COL,1):
  #    colour = image[i][j]
  #    cv.itemconfig(recList[i][j], \
  #      fill ='#%0x%0x%0x' % (colour,colour,colour),outline = '#%0x%0x%0x' % (colour,colour,colour))
  for i in xrange(0,ROW,1):
    for j in xrange(3,COL-3,1):
      colour = sobel[i][j]
      cv2.itemconfig(recList2[i][j], \
        fill ='#%0x%0x%0x' % (colour,colour,colour),outline = '#%0x%0x%0x' % (colour,colour,colour))

root.bind('<<update>>',ChangeColour)

root.mainloop()

