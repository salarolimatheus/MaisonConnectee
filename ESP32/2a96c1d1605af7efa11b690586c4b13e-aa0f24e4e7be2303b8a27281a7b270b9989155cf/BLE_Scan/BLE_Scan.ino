#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>

const int PIN = 2;
const int CUTOFF = -60;

void setup() {
  pinMode(PIN, OUTPUT);
  BLEDevice::init("");
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESP Board BLE Address:  ");
  
}

void loop() {
  BLEScan *scan = BLEDevice::getScan();
  scan->setActiveScan(true);
  BLEScanResults results = scan->start(1);
  int best = CUTOFF;
  for (int i = 0; i < results.getCount(); i++) {
    BLEAdvertisedDevice device = results.getDevice(i);
    int rssi = device.getRSSI();
    if (rssi > best) {
      best = rssi;
    }
    Serial.print(i+1);
    Serial.print(" : ");
    Serial.print(device.getAddress().toString().c_str()); 
    Serial.print(" , ");
    Serial.println(device.getName().c_str()); 
    
  }
  digitalWrite(PIN, best > CUTOFF ? Serial.println("HIGH") : Serial.println("LOW"));
}
