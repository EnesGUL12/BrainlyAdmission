#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <main.h>

MFRC522 rfid(SS_PIN, RST_PIN);

byte code[2][4] = {
                {139,215,218,43}, // Test
                {45,191,19,211} // Enes
                  };

String person[2] = {
                "Test",
                "Enes"
                  };
int id_person;
String id_str;
bool flag_online;





void RFIDSetup(){
    SPI.begin();
	rfid.PCD_Init();
    rfid.PCD_DumpVersionToSerial();
	Serial.println("RFID is active");
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

                    // Что делаем если код верный
                    if(r >= 3){
                        flag_online = true;
                        id_person = i;
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