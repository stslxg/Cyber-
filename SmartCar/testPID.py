import serial
import threading

#########################################################

ser = serial.Serial('/dev/ttyUSB0',115200,timeout = None)
hasOver = False
hasStarted = False
dataList = []
count = 0
overCount = 100
lastOutput = 0

########################################################

def Testcontroller(value):
  global count
  
  count += 1

  if count < 200:
    return 100
  elif count < 300:
    return 80
  elif count < 500:
    return 20
  elif count < 600:
    return 30
  elif count < 700:
    return 20
  elif count < 800:
    return 40
  elif count < 1000:
    return 100
  else:
    return 0

#########################################################
def Control():
  global hasOver,hasStarted,overCount,lastOutput

  while (hasOver == False) or (overCount > 0):
    #print "Waiting for ..."
    s = ser.read()
    s = ord(s) - ord('0')
    value = 0
    for i in xrange(0,s,1):
      t = ser.read()
      if i == 0:
        if t == '-' :
          sgn = -1
        else:
          sgn = 1
      if not((i == 0) and (t == '-')):  
        value = value * 10 + ord(t) - ord('0')
    
    value = value * sgn
    dataList.append(value)
    print "read...",value

    #if hasStarted == False:
    #  continue 
    #if hasOver == True:
    #  overCount -= 1
    #  output = 0
    #else :
    #  output = Testcontroller(value)  ####  <-------  Here !

    #if output != lastOutput:
    #  s = str(output)
    #  ser.write(str(len(s)))
    #  ser.write(s)
    #lastOutput = output


#########################################################
control = threading.Thread(target = Control, args = ())
control.setDaemon(True)
control.start()

ch = raw_input()
hasStarted = True
print "Started..."

ch = raw_input()
overCount = 100
hasOver = True
print "Over..."
while overCount > 0:
  pass

while dataList[0] == 0 and dataList[1] == 0:
  del dataList[0]
f = open('./pid.txt','w')
for item in dataList:
  f.write("%d " % item)
f.write("\n")

print "Record finished..."


