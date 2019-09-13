#include <Arduino.h>
#include <OneButton.h>
#include <SoftwareSerial.h>

#include <main.h>
#include <Display.h>
#include <RFID.h>
#include <Wifi.h>

OneButton button(16, false);
SoftwareSerial SIM800(0,2);

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



void callback(char* topic, byte* payload, unsigned int length) { // Функция в которой обрабатываются все присланные команды
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");


  String strTopic = String(topic); // Строка равная топику
  String strPayload = ""; // Создаем пустую строку, которая будет хранить весь payload


  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    strPayload = strPayload + ((char)payload[i]); // присваеваем значение по столбикам, чтобы получить всю строку
  }
  Serial.println();


  if(strTopic == "picture/connect"){
    if(strPayload == "info"){
      client.publish("picture/connect", "connected");
    }
  }

  else if(strTopic == "picture/info"){
    if(strPayload == "info"){
      //client.publish("picture/info", char_hours);
      delay(5000);
      //client.publish("picture/info", char_min);
    }
  }
}



void setup() {
  Serial.begin(9600);
  SIM800.begin(9600);
  SIM800.println("AT");
  Serial.println("SIM800 is active");  

  button.attachClick(click1);

  displaySetup();

  RFIDSetup();

  setupWifi();
}



void loop() {
  if (!client.connected()){ // Если потеряли подключение
    reconnect(); // Переподключаемся
    topicSub();
    client.setCallback(callback);
  }

  if(SIM800.available()){
    Serial.write(SIM800.read());
  }
  if(Serial.available()){
    SIM800.write(Serial.read());
  }


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
  client.loop();
}