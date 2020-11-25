#ifndef TEstA_h
#define TEstA_h

#include <Arduino.h>
#include <string.h>
#include "xxtea-iot-crypt.h"
#include "BluetoothSerial.h"


class TEstA{
  public:
    //Constructor
    TEstA(int modeFound, BluetoothSerial &sBT);
    
    //TEstA Key Exchange
    uint32_t TEstA_Key();
  
    //TEstA Send (XXTEA)
    void TEstA_Send(String plaintext);
  
    //TEstA Read (XXTEA)
    String TEstA_Read();    
    
  private:
    //POW MOD
    //Compute and return (b ** e) mod m
    //For unsigned b, e, m and m > 0
    uint32_t pow_mod(uint32_t b, uint32_t e, uint32_t m);
  
    //MUL MOD
    //code to compute the remainder of two numbers multiplied together.
    uint32_t mul_mod(uint32_t a, uint32_t b, uint32_t m);
  
    //RANDOM KEYGEN 
    //generates our 8 bit private secret 'a'.
    uint32_t keyGen();
    
    //FINAL KEY VARIABLE
    uint32_t xxtea_key;
  
    //Mode
    int _mode;
  
    //prime number 
    const uint32_t prime = 2147483647;
  
    //generator
    const uint32_t generator = 16807;

    //Bluetooth Serial
    BluetoothSerial sBT;
};

#endif
