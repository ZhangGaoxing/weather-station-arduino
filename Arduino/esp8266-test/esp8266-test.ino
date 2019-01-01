#include <SoftwareSerial.h>
SoftwareSerial esp(3, 2);
const int CH_PD_PIN = 5;

void setup() {
  pinMode(CH_PD_PIN, OUTPUT);
  digitalWrite(CH_PD_PIN, HIGH);

  Serial.begin(9600);    
  esp.begin(9600); 
}

void loop() {
  while(esp.available()){
    Serial.write(esp.read());
  }
  while(Serial.available()) {
    esp.write(Serial.read());
  }
}
