# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
from numpy import arange
import gtk
import gobject
from matplotlib.backends.backend_gtkagg \
  import FigureCanvasGTKAgg as FigureCanvas
from matplotlib.backends.backend_gtkagg \
  import NavigationToolbar2GTKAgg as NavigationToolBar
import shelve
from copy import deepcopy

MAX_TIME = 900
MAX_COUNT = 0

left = []
right = []
leftPWM = []
rightPWM = []
servo = []
leftv = []
rightv = []
pos = []
count = 0

def getData():
  global count, left, right, leftv, rightv, leftPWM, rightPWM, servo, pos
  print "Reading..."
  d = shelve.open('speed%d.txt' % (count,), flag = 'r')
  left = deepcopy(d['left'])
  right = deepcopy(d['right'])
  leftv = deepcopy(d['leftv'])
  rightv = deepcopy(d['rightv'])
  leftPWM = deepcopy(d['leftPWM'])
  rightPWM = deepcopy(d['rightPWM'])
  servo = deepcopy(d['servo'])
  servo = [ (1506 + i / 8)  for i in servo]
  if d.has_key('position') :
    pos = deepcopy(d['position'])
  pos = [ -i for i in pos]
  d.close()

def getCount():
  global count,MAX_COUNT
  try:
    f = open('config.txt','r')
    MAX_COUNT = int(f.readline())
  except IOError, e:
    MAX_COUNT = 0
    print u'无文件'

  count = int(input(u"请输入您要的文件序号,范围是0~%d\n" % (MAX_COUNT - 1,)))


getCount()
getData()

win = gtk.Window()
win.connect("destroy", gtk.main_quit)
win.set_default_size(1360,700)
win.set_title("Showspeed")

drawingType = int(input(u'''请输入您所需要的显示类型：
1： 左右轮分开显示实际速度、目标速度、PWM
2： 左右轮一起显示实际速度，目标速度，显示舵机
3： 左右轮实际速度比、目标速度比、舵机
4:　舵机反应图
5:　舵机散点图
'''))

if drawingType == 1 :
####################################################################################################
  fig = plt.figure(figsize = (8,5), dpi = 80)
  ax1 = fig.add_axes([0.02,0.50,0.94,0.48])
  ax2 = ax1.twinx()
  ax3 = fig.add_axes([0.02,0,0.94,0.48])
  ax4 = ax3.twinx()
  #ax5 = fig.add_subplot(313)

  p, = ax2.plot(arange(len(leftPWM)), leftPWM, label = 'leftPWM', color = 'blue')
  l, = ax1.plot(arange(len(left)), left, label = 'currentLeft', color = 'black')
  x, = ax1.plot(arange(len(leftv)), leftv, label = 'targetLeft', color = 'red')

  q, = ax4.plot(arange(len(rightPWM)), rightPWM, label = 'rightPWM', color = 'blue')
  r, = ax3.plot(arange(len(right)), right, label = 'currentRight', color = 'black')
  y, = ax3.plot(arange(len(rightv)), rightv, label = 'targetRight', color = 'red')

  #s, = ax5.plot(arange(len(servo)), servo, label = 'servo', color = 'black')

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

elif drawingType == 2:
###########################################################################################################
  fig = plt.figure(figsize = (8,5), dpi = 80)
  ax1 = fig.add_axes([0.03,0.65,0.96,0.3])
  ax2 = fig.add_axes([0.03,0.33,0.96,0.3])
  ax3 = fig.add_axes([0.03,0.02,0.96,0.3])
  ax4 = ax3.twinx()

  #p, = ax2.plot(arange(len(leftPWM)), leftPWM, label = 'leftPWM', color = 'blue')
  l, = ax1.plot(arange(len(left)), left, label = 'currentLeft', color = 'red')
  x, = ax2.plot(arange(len(leftv)), leftv, label = 'targetLeft', color = 'red')

  #q, = ax4.plot(arange(len(rightPWM)), rightPWM, label = 'rightPWM', color = 'blue')
  r, = ax1.plot(arange(len(right)), right, label = 'currentRight', color = 'black')
  y, = ax2.plot(arange(len(rightv)), rightv, label = 'targetRight', color = 'black')

  s, = ax3.plot(arange(len(servo)), servo, label = 'servo', color = 'black')
  w, = ax4.plot(arange(len(pos)), pos, label = 'position', color = 'red')  

  ax1.legend(loc = (0.85,0.1))
  ax1.grid(True)
  ax1.set_ylim(0,700)
  ax1.set_xlim(0,MAX_TIME)
  ax1.set_autoscale_on(False)

  ax2.legend(loc = (0.85,0.1))
  ax2.grid(True)
  ax2.set_ylim(0,700)
  ax2.set_xlim(0,MAX_TIME)
  ax2.set_autoscale_on(False)

  ax3.legend()
  ax4.legend()
  ax3.grid(True)
  ax3.set_ylim(1506-200,1506+200)
  ax4.set_ylim(-84,84)
  ax3.set_xlim(0,MAX_TIME)
  ax3.set_autoscale_on(False)

elif drawingType == 3:
#############################################################################################

  fig = plt.figure(figsize = (8,5), dpi = 80)
  ax1 = fig.add_subplot(111)
  ax2 = ax1.twinx()

  s, = ax2.plot(arange(len(servo)), servo, label = 'servo', color = 'black')

  tmp1 = [ float(left[i]) / float(right[i]) for i,item in enumerate(left) if right[i] != 0 ]
  tmp2 = [ float(leftv[i]) / float(rightv[i]) for i,item in enumerate(leftv) if rightv[i] != 0 ]

  v1, = ax1.plot(arange(len(tmp1)), tmp1, label = 'currentLeft / currentRight', color = 'red')
  v2, = ax1.plot(arange(len(tmp2)), tmp2, label = 'targetleft / targetRight', color = 'blue')


  ax1.legend(loc = (0.85,0.1))
  ax2.legend(loc = (0.85,0))
  ax1.grid(True)
  ax1.set_xlim(0,MAX_TIME)
  ax1.set_ylim(0,2.5)
  ax2.set_ylim(1506-200,1506+200)
  ax2.set_xlim(0,MAX_TIME)
  ax2.set_autoscale_on(False)

elif drawingType == 4 :
###########################################################################################################
  fig = plt.figure(figsize = (8,5), dpi = 80)
  ax3 = fig.add_subplot(111)
  ax4 = ax3.twinx()

  s, = ax3.plot(arange(len(servo)), servo, label = 'servo', color = 'black')
  w, = ax4.plot(arange(len(pos)), pos, label = 'position', color = 'red')  
  
  ax3.legend()
  ax4.legend()
  ax3.grid(True)
  ax3.set_ylim(1506-200,1506+200)
  ax4.set_ylim(-84,84)
  ax3.set_xlim(0,MAX_TIME)
  ax3.set_autoscale_on(False)

elif drawingType == 5 :
###########################################################################################################
  fig = plt.figure(figsize = (8,5), dpi = 80)
  ax3 = fig.add_subplot(111)

  s = ax3.scatter(servo, pos)
  
  ax3.legend()
  ax3.grid(True)



vbox = gtk.VBox()
win.add(vbox)
canvas = FigureCanvas(fig)
vbox.pack_start(canvas, expand = True, fill = True)
toolbar = NavigationToolBar(canvas, win)
vbox.pack_start(toolbar, expand = False, fill = False)

fig.canvas.draw()


win.show_all()
gtk.main()
