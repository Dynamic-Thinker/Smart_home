#include <ESP8266WiFi.h>

const char* ssid = "Airtel_mohi_5691";
const char* password = "Air@16561";

void setup()
{
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  Serial.println();
  Serial.println("Connecting...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
}