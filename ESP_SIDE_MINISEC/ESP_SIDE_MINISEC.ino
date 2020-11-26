#include "BluetoothSerial.h"
#include "TEstA.h"

//Bluetooth enable
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  TEstA tke = TEstA(1, SerialBT);  
  //MiniSec / TEstA key exchange 
  tke.TEstA_Key(Serial);
  tke.TEstA_Send("Hello!", Serial);
  Serial.println("SENT");
}


//Nothing rn
void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}
