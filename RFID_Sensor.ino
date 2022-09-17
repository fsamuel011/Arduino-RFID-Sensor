//Viral science
//RFID
//https://www.youtube.com/watch?v=KQiVLEhzzV0

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5                   // define green LED pin
#define LED_R 4                   // define red LED pin
#define BUZZER 2                  // buzzer pin
MFRC522 mfrc522(SS_PIN, RST_PIN); // create MFRC522 instance
Servo myServo;                    // define servo name

void setup() {
  Serial.begin(9600);             // initiate serial communication
  SPI.begin();                    // initiate SPI bus
  mfrc522.PCD_Init();             // initiate MFRC522
  myServo.attach(3);              // servo pin
  myServo.write(0);               // servo start position
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  Serial.println("Put your card to the reader...");
  Serial.println();
}

void loop() {
  // look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // show UID on serial monitor
  Serial.print("UID tag: ");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message: ");
  content.toUpperCase();
  // change here the UID of the card/cards that you own
  if (content.substring(1) == "63 D7 DA 1A") { 
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo.write(180);
    delay(5000);
    myServo.write(0);
    digitalWrite(LED_G, LOW);
  } else {
    Serial.println(" Access denied");
    Serial.println();
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(1000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
}
