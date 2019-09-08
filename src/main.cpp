#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneButton.h>
#include <MFRC522.h>

#define SS_PIN 12
#define RST_PIN 14

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
OneButton button(16, false);
MFRC522 mfrc522(SS_PIN, RST_PIN);



void click1(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.print("ON");
  display.display();
}



void setup() {
  Serial.begin(9600);

  button.attachClick(click1);

  SPI.begin();
	mfrc522.PCD_Init(); 

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  Serial.println("OLED begun");

  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

}

void loop() {
  button.tick();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("CHECKING");
  display.display(); // actually display all of the above
}