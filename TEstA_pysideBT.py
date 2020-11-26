import serial
from random import randint
#currently from pip install xxtea-py, other is just xxtea
import xxtea

'''
serialPort arg1 determined by your system,
maybe worth automating for final report
'''

serialPort = serial.Serial('COM4',9600,timeout = None)
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
            print("New A")
            print(A)
            print(int(A))
            A = int(A)
            b = randint(1000, 9999)
            B = (self.d1generator**b)%(self.d1prime)
            print("Random index B is: ", B)
            serialPort.write(bytes(str(B), "utf-8")) #not being read on the arduino/esp32 side
            key_inter = (A**b)%(self.d1prime)
            self.secret_key = self.secret_key + str(key_inter)
            print(self.secret_key)
        self.secret_key = self.secret_key[0:16]

    def TEstA_Send(self, text):
        encrypted = xxtea.encrypt(text, self.secret_key)
        self.serialPort.write(bytes(str(encrypted), "utf-8"))

    def TEstA_Read(self):
        #This section is to test what the encryption should look like ideally
        text = "Hello!"
        encrypt_data = xxtea.encrypt(text, self.secret_key)
        print(encrypt_data)
        decrypt_data = xxtea.decrypt_utf8(encrypt_data, self.secret_key)
        print(decrypt_data)

        #reading
        text = serialPort.readline()
        print(text)
        #decoding
        text = text.decode("utf-8")
        print(text)
        #trim end line character
        text = text[:-1]

        #turn to bytes
        text = bytes.fromhex(text)
        print(text)

        plaintext = xxtea.decrypt_utf8(text, self.secret_key)
        print(plaintext)
        return plaintext

t1 = TEstA(d1prime, d1generator, serialPort)
t1.TEstA_Key()
decr = t1.TEstA_Read()
print(decr)

#This just sends stuff on the serial, nothing important
while True:
    print('Message: ')
    message = input()
    serialPort.write(bytes(str(message), "utf-8"))
