#/usr/bin/python
# -*- coding: utf-8 -*-

import Tkinter as tk
import serial
import copy

COL = 172
ROW = 40
WIDTH = 7
BLACK = 0
WHITE = 120
count = 0
steer = 0


root = tk.Tk()
root.title(u'拍摄图像')
root.geometry('%dx%d+0+0' %(COL * WIDTH + 20, ROW * WIDTH + 80))

cv = tk.Canvas(root,bg = 'white',width = COL * WIDTH ,height = ROW * WIDTH)
recList = []
for i in xrange(0,ROW,1):
  tmp = []
  for j in xrange(0,COL,1):
    tmp.append(cv.create_rectangle(j*WIDTH,i*WIDTH,(j+1)*WIDTH,(i+1)*WIDTH,fill = 'black',outline = 'grey'))
  recList.append(tmp)
image = copy.deepcopy(recList)
line = [i for i in xrange(0,ROW,1)]
cv.pack()

#print black
   
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
  global count
  for i in xrange(0,ROW,1):
    for j in xrange(0,COL,1):
      colour = image[i][j]
      cv.itemconfig(recList[ROW - i - 1][COL - j - 1], \
        fill ='#%0x%0x%0x' % (colour,colour,colour))#,outline ='#%0x%0x%0x' % (colour,colour,colour))
  #for i in xrange(0,black[count][0],1):
  #  cv.itemconfig(recList[ROW - i - 1][black[count][i+1]],fill = 'red',outline = 'red')
  print "ok..."

root.bind('<<update>>',ChangeColour)

root.mainloop()

