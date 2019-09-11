#include <Arduino.h>
#include <OneButton.h>

#include <main.h>
#include <Display.h>
#include <RFID.h>

OneButton button(16, false);

unsigned long wait_timer;
unsigned long timer_welcome;

bool flag_wait = true;
bool flag_wait_timer;
bool flag_welcome_timer;



void click1(){

}



void setup() {
  Serial.begin(9600);

  button.attachClick(click1);

  displaySetup();

  RFIDSetup();
}



void loop() {
  button.tick();

  if(flag_wait){
    writeWait();
  }

  if(flag_online){ // Если мы дома
    if(flag_wait){
      display.clearDisplay();
      display.display();
      flag_wait = false;
      flag_welcome_timer = true;
    }
    if(flag_welcome_timer){
      writeWelcom();
      if(millis() - timer_welcome > WELCOME_TIMER){ // Если только что зашли, отсчитываем таймер для откючения надписи
        timer_welcome = millis();
        flag_welcome_timer = false;
        display.clearDisplay();
        display.display();
      }
    }
  }
  if(!flag_online){
	  if (rfid.PICC_IsNewCardPresent()){ // Если увидели новую карту, то читаем 
		  readRFID();
	  }
  }
}