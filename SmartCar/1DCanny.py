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
VALUE = 160

root = tk.Tk()
root.title(u'1DCanny边缘图像')
root.geometry('400x300+0+0')

cv = tk.Canvas(root,bg = 'white',width = 360,height = 240)
cv2 = tk.Canvas(root,bg = 'white',width = 360,height = 240)
recList = []
recList2 = []

for i in xrange(0,ROW,1):
  tmp = []
  tmp2 = []
  for j in xrange(0,COL,1):
    tmp.append(cv.create_rectangle(j*4,i*4,(j+1)*4,(i+1)*4,fill = 'black'))
    tmp2.append(cv2.create_rectangle(j*4,i*4,(j+1)*4,(i+1)*4,fill = 'black'))
  recList.append(tmp)
  recList2.append(tmp2)
image = copy.deepcopy(recList)
canny = copy.deepcopy(recList2)
canny2 = copy.deepcopy(canny)
canny3 = copy.deepcopy(canny)

#cv.pack()
cv2.pack()

##################################################################################

alpha = 1.0
omega = 0.0001

k = (1-2*exp(-alpha)*cos(omega)+exp(-2*alpha))*(alpha*alpha+omega*omega)\
    /(2*alpha*exp(-alpha)*sin(omega)+omega-omega*exp(-2*alpha))
c1 = (k*alpha)/(alpha*alpha+omega*omega)
c2 = (k*omega)/(alpha*alpha+omega*omega)
print "alpha = %f, omega = %f,k = %f" % (alpha,omega,k)

a0 = c2
a1 = (-c2*cos(omega)+c1*sin(omega))*exp(-alpha)
b1 = -2*exp(-alpha)*cos(omega)
a2 = a1 - c2*b1
b2 = exp(-2*alpha)
a3 = -c2*b2

yplus = [i for i in xrange(0,COL,1)]
yminus = copy.deepcopy(yplus)

def CalcCanny():
  global a0,a1,a2,a3,b1,b2,VALUE

  for i in xrange(0,ROW,1):
    for j in xrange(2,COL,1):
      yplus[j] = a0*image[i][j]+a1*image[i][j-1]-b1*yplus[j-1]-b2*yplus[j-2]
    for j in xrange(COL-3,-1,-1):
      yminus[j] = a2*image[i][j+1]+a3*image[i][j+2]-b1*yminus[j+1]-b2*yminus[j+2]
    for j in xrange(0,COL,1):
      canny2[i][j] = yplus[j]+yminus[j]
      #if canny[i][j] < VALUE :
      #  canny[i][j] = BLACK
      #else :
      #  canny[i][j] = WHITE
    for j in xrange(COL-2,1,-1):
      canny3[i][j] = (canny2[i][j+1]-canny2[i][j-1])*(canny2[i][j+1] - canny2[i][j-1])
    for j in xrange(COL-2,1,-1):
      if (canny3[i][j] > VALUE) and  (canny3[i][j]>canny3[i][j-1]) and (canny3[i][j]>canny3[i][j+1]) :
        canny[i][j] = WHITE
      else :
        canny[i][j] = BLACK

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
  CalcCanny()
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
      colour = canny[i][j]
      cv2.itemconfig(recList2[i][j], \
        fill ='#%0x%0x%0x' % (colour,colour,colour),outline = '#%0x%0x%0x' % (colour,colour,colour))

root.bind('<<update>>',ChangeColour)

root.mainloop()

