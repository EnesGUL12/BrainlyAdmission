#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);



void displaySetup(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  Serial.println("Display is active");
  display.clearDisplay();
  display.display();
}


void writeText(String text, int x, int y){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.print(text);
  display.display();
}



void writeExit(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,20);
  display.print("EXIT");
  display.display();
}



void writeAtHome(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,20);
  display.print("SMB");
  display.setCursor(10,35);
  display.print("AT HOME");
  display.display();
}



void writeWait(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,0);
  display.print("ATTACH");
  display.display();
}



void writeWelcom(String person){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,0);
  display.print("WELCOME");
  display.setCursor(40,20);
  display.print(person);
  display.display();
}