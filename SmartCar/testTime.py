import serial

ser = serial.Serial('/dev/ttyUSB0',115200,timeout = None)

ser.write("1")
f= open('./a.txt','w')
count = 0
for i in xrange(0,1000,1):
  a = ser.read()
  b = ser.read()
  if a == chr(0) :
    f.write("%d \nField  ,last %f\n" % (count, ord(b)*0.1))
    count = 0
  else :
    f.write("VertSyn,last %f\n" % (ord(b)*0.1))
    count = count + 1

