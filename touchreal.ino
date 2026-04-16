#include <WiFi.h>
#include <HTTPClient.h>

#define LED_PIN 2
#define TOUCH_PIN T0

const char* ssid = "iPhone";
const char* password = "forwhatdude";

String apiKey = "ZG8LHNVZLD1JZMI3";
const char* server = "http://api.thingspeak.com/update";

int threshold = 40;   // Adjust based on your touch value

void setup() {

  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Starting ESP32...");

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

}


void loop() {

  int touchValue = touchRead(TOUCH_PIN);

  Serial.print("Touch Value: ");
  Serial.println(touchValue);

  int state = 0;

  if (touchValue < threshold) {
    digitalWrite(LED_PIN, HIGH);   // LED ON when touched
    Serial.println("TOUCHED");
  }
  else {
    digitalWrite(LED_PIN, LOW);    // LED OFF when not touched
    Serial.println("NOT TOUCHED");
  }

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String url = server;
    url += "?api_key=" + apiKey;
    url += "&field1=" + String(touchValue);

    Serial.println("Sending Data to ThingSpeak...");

    http.begin(url);
    int httpCode = http.GET();

    Serial.print("HTTP Response Code: ");
    Serial.println(httpCode);

    http.end();
  }

  Serial.println("Waiting 15 seconds...\n");

  delay(15000);   // ThingSpeak requires 15 seconds delay
}