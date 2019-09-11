#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneButton.h>
#include <MFRC522.h>

#define SS_PIN 15
#define RST_PIN 0

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
OneButton button(16, false);
MFRC522 rfid(SS_PIN, RST_PIN);

byte code[2][4] = {
                {101,29,31,25},
                {45,191,19,211} // Enes
                  };
String id_str;
bool flag_online;


void click1(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.print("ON");
  display.display();
}



void readRFID()
{
  rfid.PICC_ReadCardSerial(); // Считываем данные

  Serial.println("Scanned PICC's UID:"); 
  id_str= String(rfid.uid.uidByte[0])+" "+String(rfid.uid.uidByte[1])+" "+String(rfid.uid.uidByte[2])+ " "+String(rfid.uid.uidByte[3]); // Создаем строку с id
  Serial.println(id_str);

  if(!flag_online){ // Если не активировали
    for(int i = 0; i <= 1; i++){ // Проверяем все аккаунты
      for(int r = 0; r <= 3;){
        if(rfid.uid.uidByte[r] != code[i][r]){ // Если цифры не совпадают то останавливем и начинаем проверять следущий
          break;
        }
        else if(rfid.uid.uidByte[r] == code[i][r]){ // Если цифры совпадают то активируем
          r++;
          Serial.print("num: "); Serial.print(r); Serial.println(" is true");
          if(r >= 3){
            flag_online = true;
            Serial.println("Activated");
            break;
          }
          
        }
      }
    }
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}





void setup() {
  Serial.begin(9600);

  button.attachClick(click1);

	SPI.begin();
	rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial();
	Serial.println("RFID is active");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  Serial.println("Display is active");
  display.clearDisplay();
  display.display();
}

void loop() {
  button.tick();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("CHECKING");
  display.display();
  
	if (rfid.PICC_IsNewCardPresent()){ // Если увидели новую карту, то читаем 
		readRFID();
	}
}