#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold12pt7b.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

bool direct_up;

unsigned long timer_power;

int power_i;



void displaySetup(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  Serial.println("Display is active");
  display.clearDisplay();
  display.display();
}


void writeText(String text,int size, int x, int y){
  display.setTextSize(size);
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



void writeLogo(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setFont(&FreeMonoBold12pt7b);
  display.setCursor(20,30);
  display.print("WhoMen");
  display.setCursor(0,50);
  display.print("Technolog");
  display.display();
}



void drawPower(){
  if(direct_up){
    if(millis() - timer_power > 10){
      timer_power = millis();
      power_i++;
      if(power_i >= 5){
        direct_up = false;
      }
    }
  }
  else{
    if(millis() - timer_power > 10){
      timer_power = millis();
      power_i--;
      if(power_i <= 0){
        direct_up = true;
      }
    }
  }
  display.drawCircle(display.width() - 10, 10, power_i, INVERSE);
  display.display();
}
  