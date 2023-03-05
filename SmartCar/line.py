# -*- coding: utf-8 -*-

import Tkinter as tk
import serial
import threading
from time import sleep
import copy

COL = 75
ROW = 40
WIDTH = 8
BLACK = 0
WHITE = 255
count = 0
steer = 0

ser = serial.Serial('/dev/rfcomm0',115200,timeout = None)

root = tk.Tk()
root.title(u'黑线图像')
root.geometry('%dx%d+0+0' %(COL * WIDTH + 20, ROW * WIDTH + 40))

cv = tk.Canvas(root,bg = 'white',width = COL * WIDTH ,height = ROW * WIDTH)
recList = []
for i in xrange(0,ROW,1):
  tmp = []
  for j in xrange(0,COL,1):
    tmp.append(cv.create_rectangle(j*WIDTH,i*WIDTH,(j+1)*WIDTH,(i+1)*WIDTH,fill = 'black'))
  recList.append(tmp)
image = copy.deepcopy(recList)
line = [i for i in xrange(0,ROW,1)]
score = copy.deepcopy(line)
cv.pack()

def capture():
  global count 
  f = open('./line%d.txt' % count,'w')
  count = count + 1
  for i in xrange(0,ROW,1):
    f.write("%d " % line[i])
  f.write("\n")
  print "Capture finished..."

capButton = tk.Button(root,text = u" 保存数据 ",command = capture)
capButton.pack()

def ChangeColour(event):
  global steer
  for i in xrange(0,ROW,1):
    for j in xrange(0,COL,1):
   #   if image[i][j] < BLACK :
   #     colour = 0
   #   elif image[i][j] > WHITE :
   #     colour = 255
   #   else :
   #     colour = int(float(image[i][j]-BLACK) / float(WHITE - BLACK) * 255.0)
       colour = WHITE 
       cv.itemconfig(recList[ROW - i - 1][COL - j - 1], \
        fill ='#%0x%0x%0x' % (colour,colour,colour),outline = '#%0x%0x%0x' % (colour,colour,colour))
  for i in xrange(0,ROW,1):
    if line[i] < COL:
      cv.itemconfig(recList[ROW - i - 1][COL - line[i] - 1], \
        fill ='black' ,outline = 'black')
    if score[i] < COL:
      cv.itemconfig(recList[ROW - i - 1][COL - score[i] - 1], \
        fill ='red' ,outline = 'red')

    #cv.itemconfig(recList[i][steer], \
      #fill ='blue' ,outline = 'blue')
  #print steer
 # print "ok..."
 
def GetInput():
  global steer
  while True:
    ser.write("b")
    #for i in xrange(0,ROW,1):
    #  for j in xrange(0,COL,1):
    #    s = ser.read()
    #    image[i][j] = ord(s)
    for i in xrange(0,ROW,1):
      s = ser.read()
      line[i] = ord(s)
      s = ser.read()
      score[i] = ord(s)
    #s = ser.read()
    #steer = ord(s)
    root.event_generate('<<update>>',when = 'tail')
    sleep(0.2)

root.bind('<<update>>',ChangeColour)

getinput = threading.Thread(target = GetInput,args = ())
getinput.setDaemon(True)
getinput.start()

root.mainloop()

