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

  tke.TEstA_Key();
}

void loop() {
  tke.TEstA_send("hey");
  tke.TEstA_read();
}
