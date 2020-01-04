import serial
ser = serial.Serial('/dev/ttyUSB0', 9600) #change as per your tty #-> check using ls /dev/ttty* 
while 1: 
    if(ser.in_waiting >0):
        line = ser.readline()
        print(line)