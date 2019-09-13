#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

const char* ssid = "ERRS"; // Название WIFI
const char* password = "enes5152"; // Код от WIFI
const char* mqtt_server = "192.168.1.112"; // IP mqtt сервера



void setupWifi(){ // Настройка WIFI
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
}



void reconnect() { // Если мы потеряли подключение то включаем перезагрузку
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void topicSub(){ // Подписываемся на топики
  client.subscribe("picture/connect");
  client.subscribe("picture/info");
  client.subscribe("picture/brightness");
  client.subscribe("picture/auto");
  client.subscribe("picture/save");

  client.subscribe("picture/pic/effect");
  client.subscribe("picture/effect/speed");
  client.subscribe("picture/pic/effect/color");
  client.subscribe("picture/pic/effect/brightness");

  client.subscribe("picture/effect_one_color");
  client.subscribe("picture/red");
  client.subscribe("picture/green");
  client.subscribe("picture/blue");

  client.subscribe("picture/wall/effect");
  client.subscribe("picture/wall/effect/color");
  client.subscribe("picture/wall/effect/brightness");

  client.subscribe("picture/auto/start/hour");
  client.subscribe("picture/auto/start/min");
  client.subscribe("picture/auto/end/hour");
  client.subscribe("picture/auto/end/min");

  client.subscribe("picture/pic/auto");
  client.subscribe("picture/pic/auto/place");
  client.subscribe("picture/pic/auto/timer");
  client.subscribe("picture/pic/auto/effect");
  
  client.subscribe("picture/wall/auto");
  client.subscribe("picture/wall/auto/timer");
  client.subscribe("picture/wall/auto/place");
  client.subscribe("picture/wall/auto/effect");
}