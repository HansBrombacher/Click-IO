#include "config.h"

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#define SS_PIN 15
#define RST_PIN 16

// #define SS_PIN D8
// #define RST_PIN D3
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

AdafruitIO_Feed *Poles_Standing = io.feed("Poles_Standing");
String tag;

void setup() {
  Serial.begin(115200);  // Initiate a serial communication


  while (!Serial)
    ;

  Serial.print("Connecting to Adafruit IO");
  io.connect();

  SPI.begin();         // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  // wait for serial monitor to open

  // connect to io.adafruit.com

  // wait for a connection

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}


void loop() {

  io.run();

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  tag = content.substring(1);

  Poles_Standing->save(tag);

  delay(1000);
}
