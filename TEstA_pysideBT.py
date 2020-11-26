import serial
from random import randint
import xxtea

'''
serialPort arg1 determined by your system,
maybe worth automating for final report
'''

serialPort = serial.Serial('COM4', 9600, timeout = None)
d1prime = 2147483647
d1generator = 16807
#D1P and D1G are public

class TEstA:

    def __init__(self, d1prime, d1generator, serialPort):
        self.d1prime = d1prime
        self.d1generator = d1generator
        self.serialPort = serialPort
        self.secret_key = ''

    def TEstA_Key(self):
        print('here')
        self.serialPort.write(bytes(str("start"), "utf-8"))
        for i in range(2):
            A = serialPort.readline()
            A = int(A)
            b = randint(1000, 9999)
            B = (self.d1generator**b)%(self.d1prime)
            serialPort.write(bytes(str(B), "utf-8")) #not being read on the arduino/esp32 side
            key_inter = (A**b)%(self.d1prime)
            self.secret_key = self.secret_key + str(key_inter)
            print(self.secret_key)
        self.secret_key = self.secret_key[0:16]

    def TEstA_Send(self, text):
        enckey = self.secret_key.encode()
        encrypted = xxtea.encrypt_hex(text, enckey, padding = False)
        encrypted = encrypted.decode()
        encrypted = encrypted.upper()
        self.serialPort.write(bytes(str(encrypted), "utf-8"))

    def TEstA_Read(self):
        text = serialPort.readline()
        text = text.decode("utf-8")
        text = text[:-1]
        text = text.lower()
        plaintext = xxtea.decrypt_hex(text.encode(), self.secret_key.encode(), padding=False)
        return plaintext.decode("ascii")

t1 = TEstA(d1prime, d1generator, serialPort)
t1.TEstA_Key()

serialPort.reset_output_buffer()
serialPort.reset_input_buffer()
#This just sends stuff on the serial
while True:
    foundBytes = serialPort.inWaiting()
    decr = ''
    if foundBytes > 0:
        decr = t1.TEstA_Read()
        print(decr)
    if decr == "Close":
        break

while True:
    print('Message: ')
    message = input()
    if message != "nnn":
        t1.TEstA_Send(message)
    else:
        break
