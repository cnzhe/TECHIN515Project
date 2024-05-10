#include "Secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <Wire.h>  
#include "Adafruit_VL53L0X.h" 
#include "HX711.h"

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

// VL53L0X Setup
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// HX711 Setup
const int LOADCELL_DOUT_PIN = 7; 
const int LOADCELL_SCK_PIN = 8; 
HX711 scale;

void connectAWS() { 
  delay(1000);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  client.setServer(AWS_IOT_ENDPOINT, 8883);
  client.setCallback(messageHandler);
 
  while (!client.connect(THINGNAME)) {
      delay(100);
  }

  if (!client.connected()) {
    return;
  }

  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
}
 
void publishMessage(float distance, float weight) {
  StaticJsonDocument<200> doc;
  doc["distance_mm"] = distance;
  doc["weight_g"] = weight;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void messageHandler(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
}

void setup() {
  connectAWS();

  // Initialize I2C for VL53L0X
  Wire.begin(4, 3); // SDA on GPIO4, SCL on GPIO3
  if (!lox.begin()) {
    while (1);
  }

  // Initialize HX711
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(231.85); // Calibration factor
  scale.tare(); // Reset the scale to zero
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  float distance_mm = (measure.RangeStatus != 4) ? measure.RangeMilliMeter : 0.0;
  float weight_g = scale.get_units();
  if (weight_g < 0) {
    weight_g = 0.0;
  }
  publishMessage(distance_mm, weight_g);
  client.loop();
  delay(1000);
}