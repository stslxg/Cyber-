import serial
import matplotlib.pyplot as plt
from time import sleep
import threading
from numpy import arange
import gtk
import gobject
from matplotlib.backends.backend_gtkagg \
  import FigureCanvasGTKAgg as FigureCanvas
from matplotlib.backends.backend_gtkagg \
  import NavigationToolbar2GTKAgg as NavigationToolBar
from copy import deepcopy
import shelve

MAX_TIME = 900

ser = serial.Serial('/dev/rfcomm1',115200,timeout = None)


left = []
right = []
leftPWM = []
rightPWM = []
servo = []
leftv = []
rightv = []
pos = []

count = 0

win = gtk.Window()
win.connect("destroy", gtk.main_quit)
win.set_default_size(1360,700)
win.set_title("speed")

fig = plt.figure(figsize = (8,5), dpi = 80)
ax1 = fig.add_axes([0.02,0.50,0.94,0.48])
ax2 = ax1.twinx()
ax3 = fig.add_axes([0.02,0,0.94,0.48])
ax4 = ax3.twinx()
#ax5 = fig.add_subplot(313)

p, = ax2.plot([], leftPWM, label = 'leftPWM', color = 'blue')
l, = ax1.plot([], left, label = 'currentLeft', color = 'black')
x, = ax1.plot([], leftv, label = 'targetLeft', color = 'red')

q, = ax4.plot([], rightPWM, label = 'rightPWM', color = 'blue')
r, = ax3.plot([], right, label = 'currentRight', color = 'black')
y, = ax3.plot([], rightv, label = 'targetRight', color = 'red')

#s, = ax5.plot([], servo, label = 'servo', color = 'black')
#w, = ax5.plot([], pos, label = 'position', color = 'red')

ax1.legend(loc = (0.85,0.1))
ax2.legend(loc = (0.85,0))
ax1.grid(True)
ax1.set_ylim(0,700)
ax1.set_xlim(0,MAX_TIME)
ax2.set_ylim(-32767,32767)
ax1.set_autoscale_on(False)

ax3.legend(loc = (0.85,0.1))
ax4.legend(loc = (0.85,0))
ax3.grid(True)
ax3.set_ylim(0,700)
ax3.set_xlim(0,MAX_TIME)
ax4.set_ylim(-32767,32767)
ax3.set_autoscale_on(False)

#ax5.legend()
#ax5.set_ylim(1506-200,1506+200)
#ax5.set_xlim(0,MAX_TIME)
#ax5.set_autoscale_on(False)


def getData():
  while True:
    s = ser.read()
    if s == 'l' :
      s = int(ser.read())
      left.append(int(ser.read(s)))
    if s == 'r' :
      s = int(ser.read())
      right.append(int(ser.read(s)))
    if s == 'x' :
      s = int(ser.read())
      leftv.append(int(ser.read(s)))
    if s == 'y' :
      s = int(ser.read())
      rightv.append(int(ser.read(s)))
    if s == 'p' :
      s = int(ser.read())
      leftPWM.append(int(ser.read(s)))
    if s == 'q' :
      s = int(ser.read())
      rightPWM.append(int(ser.read(s)))
    if s == 's' :
      s = int(ser.read())
      servo.append(int(ser.read(s)))
    if s == 'w' :
      s = int(ser.read())
      pos.append(int(ser.read(s)))

    sleep(0)


def updatePlot(*args):
  l.set_data(arange(len(left)), left)
  r.set_data(arange(len(right)), right)
  x.set_data(arange(len(leftv)), leftv)
  y.set_data(arange(len(rightv)), rightv)
  p.set_data(arange(len(leftPWM)), leftPWM)
  q.set_data(arange(len(rightPWM)), rightPWM)
  #s.set_data(arange(len(servo)), servo)
  #w.set_data(arange(len(pos)), pos)
  
  fig.canvas.draw()
  sleep(0.2)
  return True

updatePlot()

readData = threading.Thread(target = getData, args = ())
readData.setDaemon(True)
readData.start()

#vbox = gtk.VBox()
#win.add(vbox)
canvas = FigureCanvas(fig)
#vbox.pack_start(canvas, expand = True, fill = True)
#toolbar = NavigationToolBar(canvas, win)
#vbox.pack_start(toolbar, expand = False, fill = False)

win.add(canvas)


gobject.idle_add(updatePlot)
win.show_all()

def getCount():
  global count
  try:
    f = open('config.txt','r')
    count = int(f.readline())
  except IOError, e:
    count = 0

getCount()
ser.write("s")

gtk.main()
