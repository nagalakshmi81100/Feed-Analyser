#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// ===============================
// YOUR INTERNET Wi-Fi / HOTSPOT
// ===============================
const char* ssid = "Nagalakshmi";
const char* password = "priya123";

// ===============================
// FIREBASE DATABASE
// ===============================
const char* firebaseURL =
  "https://feed-analyser-f30bd-default-rtdb.firebaseio.com/sensorData.json";

// Fake sensor values
float pH;
float NIR;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Starting ESP8266...");

  // Connect ESP8266 to Internet Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi Connected!");

  Serial.print("ESP8266 IP Address: ");
  Serial.println(WiFi.localIP());

  randomSeed(micros());
}

void loop()
{
  // Generate fake sensor values
  pH = random(55, 86) / 10.0;
  NIR = random(0, 1001) / 10.0;

  Serial.println();
  Serial.println("------ Sensor Data ------");

  Serial.print("pH  : ");
  Serial.println(pH, 1);

  Serial.print("NIR : ");
  Serial.println(NIR, 1);

  // Send data to Firebase
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClientSecure client;

    // For testing only
    client.setInsecure();

    HTTPClient http;

    http.begin(client, firebaseURL);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{";
    jsonData += "\"pH\":";
    jsonData += String(pH, 1);
    jsonData += ",";
    jsonData += "\"NIR\":";
    jsonData += String(NIR, 1);
    jsonData += "}";

    Serial.print("Sending to Firebase: ");
    Serial.println(jsonData);

    int httpResponseCode = http.PUT(jsonData);

    Serial.print("Firebase Response Code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0)
    {
      Serial.println("Firebase Upload SUCCESS!");
    }
    else
    {
      Serial.println("Firebase Upload FAILED!");
    }

    http.end();
  }
  else
  {
    Serial.println("Wi-Fi disconnected!");
  }

  // Send new data every 5 seconds
  delay(5000);
}