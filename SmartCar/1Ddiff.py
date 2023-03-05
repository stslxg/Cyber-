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
VALUE = 40

root = tk.Tk()
root.title(u'1DDiff边缘图像')
root.geometry('720x520+0+0')

cv = tk.Canvas(root,bg = 'white',width = 360,height = 240)
cv2 = tk.Canvas(root,bg = 'white',width = 720,height = 480)
recList = []
recList2 = []

for i in xrange(0,ROW,1):
  tmp = []
  tmp2 = []
  for j in xrange(0,COL,1):
    tmp.append(cv.create_rectangle(j*4,i*4,(j+1)*4,(i+1)*4,fill = 'black'))
    tmp2.append(cv2.create_rectangle(j*8,i*8,(j+1)*8,(i+1)*8,fill = 'black'))
  recList.append(tmp)
  recList2.append(tmp2)
image = copy.deepcopy(recList)
diff = copy.deepcopy(recList2)
diffx = copy.deepcopy(diff)


#cv.pack()
cv2.pack()

##################################################################################

def CalcDiff():
  global a0,a1,a2,a3,b1,b2,VALUE

  minV = 10000
  maxV = -10000
  for i in xrange(1,ROW-1,1):
    for j in xrange(2,COL-1,1):
      diffx[i][j] = abs(image[i][j] - image[i][j-2])
  for i in xrange(1,ROW-1,1):
    for j in xrange(1,COL-1,1):
      if (diffx[i][j] > VALUE) and (diffx[i][j] > diffx[i][j-1]) and (diffx[i][j] > diffx[i][j+1]) :
        diff[i][j] = WHITE
      else : 
        diff[i][j] =BLACK
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
  CalcDiff()
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
      colour = diff[i][j]
      cv2.itemconfig(recList2[i][j], \
        fill ='#%0x%0x%0x' % (colour,colour,colour),outline = '#%0x%0x%0x' % (colour,colour,colour))

root.bind('<<update>>',ChangeColour)

root.mainloop()

