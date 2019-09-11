#include <Arduino.h>
#include <OneButton.h>

#include <main.h>
#include <Display.h>
#include <RFID.h>

OneButton button(16, false);

unsigned long timer_welcome;

bool flag_wait = true;
bool flag_welcome;
bool flag_at_home;



void click1(){
  if(!flag_online && !flag_wait){
    flag_wait = true;
  }
  if(flag_at_home){
    flag_online = false;
    flag_at_home = false;
    display.clearDisplay();
    writeExit();
    delay(1000);
    display.clearDisplay();
  } 
}



void setup() {
  Serial.begin(9600);

  button.attachClick(click1);

  displaySetup();

  RFIDSetup();
}



void loop() {
  button.tick();
  display.display();

  if(flag_wait){
    writeWait();
    if(rfid.PICC_IsNewCardPresent()){ // Если увидели новую карту, то читаем 
		  readRFID();
	  }
  }

  if(flag_online){ // Если мы дома
    if(flag_wait){ // Если был включен режим ожидания
      display.clearDisplay();
      display.display();
      flag_wait = false;
      flag_welcome = true;
      timer_welcome = millis();
    }

    else if(flag_welcome){
      writeWelcom(person[id_person]);
      if(millis() - timer_welcome > WELCOME_TIMER){ // Если только что зашли, отсчитываем таймер для откючения надписи
        timer_welcome = millis();
        flag_welcome = false;
        flag_at_home = true;
        display.clearDisplay();
        display.display();
      }
    }

    else if(flag_at_home){ // Если мы дома то показываем на дисплее, что кто то дома 
      writeAtHome();
    }
  }
}