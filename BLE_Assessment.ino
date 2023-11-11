#include <BLEDevice.h>
#ifndef LED_BUILTIN
#define LED_BUILTIN (2) // LED Pin on DevKit V1 boards
#endif

BLEUUID serviceID("5e89440c-7fda-11ee-b962-0242ac120002");
BLEUUID readonlyCharID("5e894812-7fda-11ee-b962-0242ac120002");
BLEUUID writableCharID("2b7d3514-7fe4-11ee-b962-0242ac120002");

// read-only characteristic for ellapsed time
BLEUUID ellapsedTimeReadOnlyID("66869ab8-8095-11ee-b962-0242ac120002");

// store for our writeable data
uint8_t writable_store[1];

// set time for initialisation of program
unsigned long start = millis();

// bluetooth callback handler
class MyCallbacks: public BLECharacteristicCallbacks { // inheritence syntax
  void onWrite(BLECharacteristic *pCharacteristic) {

    // if the characteristic matches our writable one
    if (writableCharID.equals(pCharacteristic->getUUID())) {
      // retrieve the data
      uint8_t *value = pCharacteristic->getData();
      digitalWrite(LED_BUILTIN, value[0]?HIGH:LOW);
    }
  }
};

// create callback handler 
MyCallbacks cb; 

// set the ellapsed time characteristic variable to be global so it can be
// periodacally updated in the loop func
BLECharacteristic *ellapsedCharacteristic;

void setup() {
  // Enable the LED
  pinMode(LED_BUILTIN, OUTPUT);

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

  // create the ellapsed time characteristic
  ellapsedCharacteristic = pService->createCharacteristic(
    ellapsedTimeReadOnlyID, 
    BLECharacteristic::PROPERTY_READ
  );


  readCharacteristic->setValue("Arduino says hi!");

  //An example writable characteristic
  BLECharacteristic *writeCharacteristic = pService->createCharacteristic(
  writableCharID,
  BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  //bit-wise OR is used to enable both properties.
  writeCharacteristic ->setValue(writable_store, 1);
  writeCharacteristic ->setCallbacks(&cb);


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

  // calculate ellapsed time since start of program
  unsigned long ellapsed = millis() - start;

  // convert to seconds
  unsigned long ellapsedInSeconds = ellapsed / 1000;
}
