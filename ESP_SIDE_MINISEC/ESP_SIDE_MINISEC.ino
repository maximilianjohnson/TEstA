#include "BluetoothSerial.h"
#include "TEstA.h"

//Bluetooth enable
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
TEstA tke = TEstA(1, SerialBT); 

void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32test"); //Bluetooth device name
   
  //MiniSec / TEstA key exchange 
  tke.TEstA_Key(Serial);
}


//Nothing rn
void loop() {
  if (Serial.available()) {
    String message = Serial.readString();
    tke.TEstA_Send(message, Serial);
    Serial.println("SENT");
    Serial.flush();
  }
  if (SerialBT.available()) {
    String message = tke.TEstA_Read(Serial);
    Serial.println(message);
  }
  delay(20);
}
