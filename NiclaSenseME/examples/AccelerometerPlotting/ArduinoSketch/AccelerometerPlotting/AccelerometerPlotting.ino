#include <ArduinoBLE.h>
#include "Arduino_BHY2.h"

BLEService accelerometerService("19b10000-e8f2-537e-4f6c-d104768a1214");

BLEStringCharacteristic firstCharacteristic("19b10010-e8f2-537e-4f6c-d104768a1214",  
    BLERead | BLENotify, 50); 

SensorXYZ accel(SENSOR_ID_ACC);
String oldAcclerometerReading = "0";


#define DEBUG 0

void setup() {
  Serial.begin(115200);    // initialize serial communication
  while (!Serial);

  BHY2.begin();
  accel.configure(25, 0);
  
  BLE.setAdvertisedService(accelerometerService); // add the service UUID
  accelerometerService.addCharacteristic(firstCharacteristic); // add the characteristic
  BLE.addService(accelerometerService); // Add the service
  firstCharacteristic.writeValue(oldAcclerometerReading); // set initial value for this characteristic

  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {

  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:

    // while the central is connected:
    while (central.connected()) {
      //long currentMillis = millis();
      //if (currentMillis - previousMillis >= 200) {
      //  previousMillis = currentMillis;
        BHY2.update();
        updateAcceleration();
      //}
    }
    
    Serial.print("BLE disconnected");
    Serial.println(central.address());
  }
}

void updateAcceleration() {
    float x, y, z;
    
    x = accel.x() / 4096.0f;
    y = accel.y() / 4096.0f;
    z = accel.z() / 4096.0f;
    
    String accelerometerData = String(x)+"|"+String(y)+"|"+String(z);
    Serial.println(accelerometerData);
    firstCharacteristic.writeValue(accelerometerData);
}
