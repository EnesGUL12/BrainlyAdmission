#include <Arduino.h>
#include <OneButton.h>
//#include <SoftwareSerial.h>

#include <main.h>
#include <Display.h>
#include <RFID.h>
#include <Wifi.h>

OneButton button(16, false);
//SoftwareSerial SIM800(10,9);

unsigned long timer_welcome;
unsigned long timer_wait;

bool flag_wait = true;
bool flag_welcome;
bool flag_at_home;

String str_state;
char char_state[10];

//TODO: Добавить GSM модуль

void click1(){
  if(flag_wait){ // режим ожидания
    flag_wait = false; // то выключаем режим ожидания
    str_state = "not_at_home";
    str_state.toCharArray(char_state, 10);
    client.publish("admiss/state", "mistake");
    display.clearDisplay();
    display.display();
  }
  else if(!flag_online && !flag_wait){ // если не авторизованы и не ожидаем
    flag_wait = true; // включаем режим ожидания
    timer_wait = millis();
    str_state = "wait";
    str_state.toCharArray(char_state, 10);
    client.publish("admiss/state", "wait");
  }
  else if(flag_at_home){ // если дома
    flag_online = false; // то меням на режим не дома или выход
    flag_at_home = false;
    str_state = "exit";
    str_state.toCharArray(char_state, 10);
    client.publish("admiss/state", "exit");
    display.clearDisplay();
    writeExit();
    delay(1000);
    display.clearDisplay();
    display.display();
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


  if(strTopic == "admiss/connect"){
    if(strPayload == "info"){ // Проверка подключения(только для приложения)
      client.publish("admiss/connect", "connected");
    }
  }
  else if(strTopic == "admiss/state"){ 
    if(strPayload == "info"){ // отправляем состояние
      client.publish("admiss/state", char_state);
    }
  }
}



void setup() {
  Serial.begin(9600);
  //SIM800.begin(9600);
  //SIM800.println("AT");
  //Serial.println("SIM800 is active");  

  button.attachClick(click1);

  displaySetup();

  RFIDSetup();

  setupWifi();
  setupWifiUpd();

  writeLogo();
  delay(5000);
  display.setFont();
}



void loop() {
  if (!client.connected()){ // Если потеряли подключение
    reconnect(); // Переподключаемся
    topicSub();
    client.setCallback(callback);
    writeText("ERROR",2,0,0);
    writeText("CHECK",2,0,20);
    writeText("WIFI",2,0,40);
    display.clearDisplay();
  }

  
  else{
    //if(SIM800.available()){
      //Serial.write(SIM800.read());
    //}
    //if(Serial.available()){
      //SIM800.write(Serial.read());
    //}

    button.tick();

    if(!flag_wait && !flag_online){
      drawPower();
      display.clearDisplay();
    }

    if(flag_wait){ // Режим ожидания карточки
      if(millis() - timer_wait > WAIT_TIMER_TIME){ // Если прошло больше n времени то возвращаемся в режим не дома
        timer_wait = millis();
        flag_wait = false;
        display.clearDisplay();
        display.display();
        str_state = "not_at_home";
        str_state.toCharArray(char_state, 10);
        client.publish("admiss/state", "mistake");
      }
      else{
        writeWait();
        if(rfid.PICC_IsNewCardPresent()){ // Если увидели новую карту, то читаем 
          readRFID();
        }
      }
    }

    if(flag_online){ // Если мы дома
      if(flag_wait){
        display.clearDisplay();
        display.display();
        flag_wait = false;
        flag_welcome = true;
        timer_welcome = millis();
        str_state = "at_home";
        str_state.toCharArray(char_state, 10);
        client.publish("admiss/state","enter"); // Говорим что мы вошли
      }

      else if(flag_welcome){
        writeWelcom(person[id_person]);
        if(millis() - timer_welcome > WELCOME_TIMER){ // Если только что зашли, отсчитываем таймер для откючения надписи
          timer_welcome = millis();
          flag_welcome = false;
          flag_at_home = true;
          display.clearDisplay();
          display.display();
          client.publish("admiss/state","at_home");
        }
      }

      else if(flag_at_home){ // Если мы дома то показываем на дисплее, что кто то дома 
        writeAtHome();
      }
    }
  }
  client.loop();
  ArduinoOTA.handle(); // Всегда готовы к прошивке
}