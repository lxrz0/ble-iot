#include <BLEDevice.h>
#ifndef LED_BUILTIN
#define LED_BUILTIN (2) // LED Pin on DevKit V1 boards
#endif

BLEUUID serviceID("5e89440c-7fda-11ee-b962-0242ac120002");
BLEUUID readonlyCharID("5e894812-7fda-11ee-b962-0242ac120002");
BLEUUID writeableCharID("2b7d3514-7fe4-11ee-b962-0242ac120002");

// bluetooth callback handler
class MyCallbacks: public BLECharacteristicCallbacks { // inheritence syntax
  void onWrite(BLECharacteristic *pCharacteristic) {

    // if the characteristic matches our writable one
    if (writeableCharID.equals(pCharacteristic->getUUID())) {
      // retrieve the data
      uint8_t *value = pCharacteristic->getData();
      digitalWrite(LED_BUILTIN, value[0]?HIGH:LOW);
    }
  }
};

void setup() {
  // initialse device device name
  BLEDevice::init("Christian's Arduino");

  // setup in server mode
  BLEServer *pServer = BLEDevice::createServer();

  // create service
  BLEService *pService = pServer->createService(serviceID);

  // create a read-only characteristic
  BLECharacteristic *readCharacteristic = pService->createCharacteristic(
    readonlyCharID, 
    BLECharacteristic::PROPERTY_READ
  );

  readCharacteristic->setValue("Arduino says hi!");

  // start the service 
  pService->start();

  // Advertising config
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serviceID);
  pAdvertising->setScanResponse(true);

  // iPhone connection (Advertising intervals)
  pAdvertising->setMinPreferred(0x06); // 6 * 0.625 ms
  pAdvertising->setMinPreferred(0x12); // 18 * 0.625 ms

  // Start advertising
  BLEDevice::startAdvertising();
}

void loop() {
  // save power
  delay(1000);
}
