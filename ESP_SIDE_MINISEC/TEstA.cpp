#include <Arduino.h>
#include <string.h>
#include "xxtea-iot-crypt.h"
#include "BluetoothSerial.h"
#include "TEstA.h"

//FINAL KEY VARIABLE
extern String xxtea_key;
  
//Mode
extern int _mode;
  
//prime number 
extern const uint32_t prime = 2147483647;
  
//generator
extern const uint32_t generator = 16807;

//Bluetooth Serial
extern BluetoothSerial sBT;

////Hardware Serial
//extern HardwareSerial sRef;


TEstA::TEstA(int modeFound, BluetoothSerial &sBluetooth){
  _mode = modeFound;
  sBT = sBluetooth;
}

uint32_t TEstA::keyGen() {
  //Seed the random number generator with a reading from an unconnected pin, I think this on analog pin 2
  randomSeed(analogRead(2));

  //return a random number between 1 and our prime .
  return random(1, prime);
}
 

//MUL MOD
//code to compute the remainder of two numbers multiplied together.
uint32_t TEstA::mul_mod(uint32_t a, uint32_t b, uint32_t m) {


    uint32_t result = 0; //variable to store the result
    uint32_t runningCount = b % m; //holds the value of b*2^i

    for (int i = 0; i < 32; i++) {

        if (i > 0) runningCount = (runningCount << 1) % m;
        if (bitRead(a, i)) {
            result = (result % m + runningCount % m) % m;

        }

    }
    return result;
}

//POW MOD
//Compute and return (b ** e) mod m
//For unsigned b, e, m and m > 0
uint32_t TEstA::pow_mod(uint32_t b, uint32_t e, uint32_t m)
{
    uint32_t r;  // result of this function

    uint32_t pow;
    uint32_t e_i = e;
    // current bit position being processed of e, not used except for debugging
    uint8_t i;

    // if b = 0 or m = 0 then result is always 0
    if (b == 0 || m == 0) {
        return 0;
    }

    // if e = 0 then result is 1
    if (e == 0) {
        return 1;
    }

    // reduce b mod m 
    b = b % m;

    // initialize pow, it satisfies
    //    pow = (b ** (2 ** i)) % m
    pow = b;

    r = 1;

    // stop the moment no bits left in e to be processed
    while (e_i) {
        // At this point pow = (b ** (2 ** i)) % m

        // and we need to ensure that  r = (b ** e_[i..0] ) % m
        // is the current bit of e set?
        if (e_i & 1) {
            // this will overflow if numbits(b) + numbits(pow) > 32
            r = mul_mod(r, pow, m);//(r * pow) % m; 
        }

        // now square and move to next bit of e
        // this will overflow if 2 * numbits(pow) > 32
        pow = mul_mod(pow, pow, m);//(pow * pow) % m;

        e_i = e_i >> 1;
        i++;
    }

    // at this point r = (b ** e) % m, provided no overflow occurred
    return r;
}



//MY FIRST ATTEMPT AT PUTTING IT ALL TOGETHER
//BASIC DH
//B IS GENERATED ON THE OTHER SIDE 
void TEstA::TEstA_Key(HardwareSerial &sRef) {
    while(sBT.available() == 0){
    }
    sBT.flush();
    uint32_t k[2];
    for (int i = 0; i < 2; i++){
      //This is our secret key
      uint32_t a = keyGen();
  
      //This is our shared index 'A'
      uint32_t A = pow_mod(generator, a, prime);
      String sA = String(A);
      String toSend = sA + '\n';
      sBT.print(toSend);
      sBT.flush();
  
      while (sBT.available() == 0) {
      } //wait until the there are bits in the serial
      String BinStr = sBT.readString(); // Problem 1 - FIXED
      
      char Barray[11];
      BinStr.toCharArray(Barray, 11);
      uint32_t B = strtoul(Barray, NULL, 0);
  
      sBT.flush();
  
      //This is our shared secret encryption key.
      k[i] = pow_mod(B, a, prime);
      sRef.println(k[i]);
    }
    String kS1 = String(k[0]);
    String kS2 = String(k[1]);
    String fresh_key_pre = kS1 + kS2;
    String fresh_key = fresh_key_pre.substring(0, 16);
    sRef.println(fresh_key);
    xxtea_key = fresh_key;
}


//MINISEC/ TEstA encryption 100% based on xxtea

//Sending Function
void TEstA::TEstA_Send(String plaintext, HardwareSerial &sRef) {
    // Perform Encryption on the Data
    sRef.println("xxtea_key");
    sRef.println(xxtea_key);
    xxtea.setKey(xxtea_key);
    String result = xxtea.encrypt(plaintext);
    String toSend = result + '\n';
    sRef.println(toSend);
    // Send to BT
    sBT.print(toSend);
}

//Reading Function
String TEstA::TEstA_Read(HardwareSerial &sRef) {   
    // Receive message
    xxtea.setKey(xxtea_key);
    while (sBT.available() == 0) {
    } //wait until the there are bits in the serial   
    String encd = sBT.readString(); // Problem 1 - FIXED
    sBT.flush();
    sRef.println(encd);
    // Perform Decryption
    String result = xxtea.decrypt(encd);
    sRef.println(result);
    return result;
}
