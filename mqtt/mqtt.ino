#include <WiFi.h>
#include <PubSubClient.h>

#define SSID "SKYF57F8"
#define WIFI_PWD "QWPRZFRQ"
#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_PORT (1883)
#define MQTT_PUBLIC_TOPIC "uok/iot/cl586/capacitive"
#define MQTT_SUBSCRIBE_TOPIC "uok/iot/cl586/demo"
#define TOUCH_PIN (4)

WiFiClient wifiClient;
PubSubClient client(wifiClient);
boolean touchFlag = false;

void handleEvent(char* topic, byte* payload, unsigned int length) {
  Serial.printf("Message received on: %s", topic);
  Serial.println();

  for (int i=0; i<length;i++) {
    Serial.print( (char) payload[i]);
  }

  Serial.println();
}

void setup () {
  Serial.begin(115200);
  
  // Connect to wifi
  Serial.print("Connecting to:" SSID);
  WiFi.begin(SSID, WIFI_PWD);

  // wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Handle MQTT connection
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(handleEvent);

  while (!client.connected()) {

    String clientId = "ESP32Client-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("MQTT connected");

      // subscribe to topic
      client.subscribe(MQTT_SUBSCRIBE_TOPIC);
      Serial.printf("Subscribed to: %s", MQTT_SUBSCRIBE_TOPIC);
    } else {
      Serial.printf("failed, rc=%d try again in 5 seconds", client.state());
      delay (5000);
    }
  }

}

void loop () {
  client.loop(); // let the client manage its internals

  if (touchRead(TOUCH_PIN) < 50) {
    if (!touchFlag) {
      touchFlag = true;
      Serial.println("Touched");
      client.publish(MQTT_PUBLIC_TOPIC, "touch");
    }
  } 
  
  else {
    touchFlag = false;
  }
}