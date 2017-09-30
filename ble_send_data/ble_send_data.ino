#include "CurieBle.h"

static const char* bluetoothDeviceName = "MyDevice"; 

static const char* serviceUuid = "1800";
static const char* characteristicUuid = "2803";
static const int   characteristicTransmissionLength = 20; 

BLEService service(serviceUuid); 
BLECharacteristic characteristic(
  characteristicUuid,
  BLERead, // readable from client's perspective
  characteristicTransmissionLength
);

BLEPeripheral blePeripheral; 

unsigned long lastBleCharacteristicUpdateTime_ms = 0;
unsigned long updateTimeout_ms = 5000; // update every 5 seconds


char* generateRandomString(const int length) {
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    char output[length];
    for (int i = 0; i < length; i++) {
        output[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    output[length] = '\0';
}

void setBleCharacteristicValue(char* output, int length) {
  characteristic.setValue((const unsigned char*) output, length); 
}


void setup() {
  blePeripheral.setLocalName(bluetoothDeviceName); 

  blePeripheral.setAdvertisedServiceUuid(service.uuid()); 
  blePeripheral.addAttribute(service);
  blePeripheral.addAttribute(characteristic);

  blePeripheral.begin(); 
  
  lastBleCharacteristicUpdateTime_ms = millis();
}

void loop() {
  unsigned long currentTime_ms = millis();
  if ((currentTime_ms - lastBleCharacteristicUpdateTime_ms) > updateTimeout_ms) {
    lastBleCharacteristicUpdateTime_ms = currentTime_ms;
    
    int randomStringLength = random(1, characteristicTransmissionLength);
    char* randomString = generateRandomString(randomStringLength);
    
    setBleCharacteristicValue(randomString, randomStringLength);
  }
  
}

