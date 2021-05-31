#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

int sensorvalue = 0;


class MyCallbacks : public BLECharacteristicCallbacks { // 스마트폰에서 데이터 전송시 callback
    void onWrite(BLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (touchRead(T0) > 100) {
            Serial.print("New value: ");
            for (int i = 0; i < value.length(); i++)
            Serial.print(value[i]);
            Serial.println();
        }
    }
};
void setup() {
    Serial.begin(115200);
    BLEDevice::init("MyESP32"); // 디바이스 이름
    BLEServer* pServer = BLEDevice::createServer();
    BLEService* pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic* pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );
    pCharacteristic->setCallbacks(new MyCallbacks());

  
    sensorvalue = touchRead(T0);

    //in order to send the value we must convert it to characteristic
    char txString[8];
    dtostrf(sensorvalue, 1, 2, txString);
    pCharacteristic->setValue(txString);
    //pCharacteristic->notify();
    Serial.println("Sent value : " + String(txString));  
    
    pService->start();
    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}
void loop() {
  Serial.println(touchRead(T0));  // get value using T0
  delay(1000);
}

//https://forum.arduino.cc/t/sending-data-from-esp32-with-ble-in-my-phone/676015/7
