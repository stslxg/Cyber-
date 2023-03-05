# -*- coding: utf-8 -*-

import Tkinter as tk
import serial
import copy

COL = 90
ROW = 60
BLACK = 0
WHITE = 255
count = 0

root = tk.Tk()
root.title(u'摄像头图像')
root.geometry('720x550+0+0')

cv = tk.Canvas(root,bg = 'white',width = 720,height = 480)
recList = []
for i in xrange(0,ROW,1):
  tmp = []
  for j in xrange(0,COL,1):
    tmp.append(cv.create_rectangle(j*8,i*8,(j+1)*8,(i+1)*8,fill = 'black'))
  recList.append(tmp)
image = copy.deepcopy(recList)
cv.pack()

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

def ChangeColour(event):
  global image
  for i in xrange(0,ROW,1):
    for j in xrange(0,COL,1):
      colour = image[i][j]
      cv.itemconfig(recList[i][j], \
        fill ='#%0x%0x%0x' % (colour,colour,colour),outline = '#%0x%0x%0x' % (colour,colour,colour))
  print "ok..."

root.bind('<<update>>',ChangeColour)

GetImage()
root.mainloop()

