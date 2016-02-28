#include <SerialCommand.h>

#include <IRremote.h>


#define arduinoLED 6
IRsend irsend;
SerialCommand SCmd;

#define LED_ON() digitalWrite(arduinoLED, HIGH)
#define LED_OFF() digitalWrite(arduinoLED, LOW)

void samsungPower() {
  LED_ON();
  for (int i = 0; i < 5; i++) {
    irsend.sendSAMSUNG(0xe0e040bf, 32);
    delay(100);
  }

  LED_OFF();
}

void setup()
{
  pinMode(arduinoLED,OUTPUT);
  digitalWrite(arduinoLED,LOW);
  Serial.begin(9600);

  SCmd.addCommand("sp",samsungPower);
}

void loop() {
  SCmd.readSerial();
}

