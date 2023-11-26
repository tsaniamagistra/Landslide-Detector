#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "kenji";  // Nama WiFi
const char *password = "hagiwara";  // Kata sandi WiFi
const char *serverAddress = "http://localhost:9000";  // Alamat server

const int moist_pin = A0; // Pin sensor kelembaban tanah (analog A0)
const int vibr_pin = D0; // Pin sensor getaran (digital D0)

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  int soilMoisture = analogRead(moist_pin);
  int vibration = digitalRead(vibr_pin);

  Serial.print("Soil Moisture: ");
  Serial.println(soilMoisture);
  Serial.print("Vibration: ");
  Serial.println(vibration);
  
  sendToServer(soilMoisture, vibration);

  delay(15000);
}

void sendToServer(int soilMoisture, int vibration) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client; // Buat objek WiFiClient

    String data = "moist=" + String(soilMoisture) + "&vibr=" + String(vibration);
    String serverEndpoint = String(serverAddress) + "/insert-log"; // Menggabungkan serverAddress dengan "/insert-log"

    http.begin(client, serverEndpoint);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST(data);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.println("Error sending data to server");
    }

    http.end();
  }
}
