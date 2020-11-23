import serial
from random import randint

'''
serialPort arg1 determined by your system,
maybe worth automating for final report
'''

serialPort = serial.Serial('COM4',9600,timeout = None)

#D1P and D1G are public
d1prime = 2147483647
d1generator = 16807

#Standard DH
b = randint(1000, 9999)
B = (d1generator**b)%(d1prime)

#Could write a start message, whatever our TEstA needs
serialPort.write(bytes(str("start"), "utf-8"))

#Needs to reaad A
#needs to know length of A incoming in bytes I believe
A = serialPort.read()

#this should be sending b
#B needs to be read as an uint32_t on the other side
#Part of found "Problem 1"
#Not sure how pyserial write works, how it needs to be encoded ect
serialPort.write(bytes(str(B), "utf-8")) #not being read on the arduino/esp32 side

#This just sends stuff on the serial, nothing important
while True:
    print('Message: ')
    message = input()
    serialPort.write(bytes(str(message), "utf-8"))
