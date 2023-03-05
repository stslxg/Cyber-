# -*- coding: utf-8 -*-

import Tkinter as tk
import serial
import threading
from time import sleep
import copy

COL = 50
ROW = 45
BLACK = 0
WHITE = 255

f = open('./b.txt','r')

root = tk.Tk()
root.title(u'摄像头图像')
root.geometry('400x400+100+100')

cv = tk.Canvas(root,bg = 'white')
recList = []
for i in xrange(0,ROW,1):
  tmp = []
  for j in xrange(0,COL,1):
    tmp.append(cv.create_rectangle(j*10,i*10,(j+1)*10,(i+1)*10,fill = 'black'))
  recList.append(tmp)
image = copy.deepcopy(recList)
cv.pack()

def ChangeColour(event):
  for i in xrange(0,ROW,1):
    for j in xrange(0,COL,1):
      colour = int(float(image[i][j]-BLACK)/float(WHITE-BLACK)*255) 
      cv.itemconfig(recList[i][j], \
        fill ='#%0x%0x%0x' % (colour,colour,colour),outline = '#%0x%0x%0x' % (colour,colour,colour))
  print "ok..."
 
def GetInput():  
  while True:
    ch = raw_input()
    for i in xrange(0,ROW,1):
      for j in xrange(0,COL,1):
        s = f.read(1)
        image[i][j] = ord(s)         
    root.event_generate('<<update>>',when = 'tail')
    sleep(0)

root.bind('<<update>>',ChangeColour)

getinput = threading.Thread(target = GetInput,args = ())
getinput.setDaemon(True)
getinput.start()

root.mainloop()

