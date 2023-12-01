#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

/*
 Untuk pengguna: Ganti placeholder berikut:
 - placeholder1 = Nama SSID Wi-Fi Anda
 - placeholder2 = Kata sandi Wi-Fi Anda
 - placeholder3 = Alamat IP atau nama host server yang Anda gunakan di server.js
 Pastikan bahwa NodeMCU dan server terhubung ke jaringan Wi-Fi yang sama.
*/
const char *ssid = "placeholder1"; 
const char *password = "placeholder2";
const char *serverAddress = "http://placeholder3:9000";

const int moist_pin = A0; // Pin sensor kelembaban tanah
const int vibr_pin = D0; // Pin sensor getaran

void setup() {
  Serial.begin(115200);
  delay(100);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
    
  checkServerConnection();
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
    WiFiClient client;

    String data = "moist=" + String(soilMoisture) + "&vibr=" + String(vibration);
    String serverEndpoint = String(serverAddress) + "/insert-log";

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

void checkServerConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    String serverEndpoint = String(serverAddress) + "/check-connection";

    http.begin(client, serverEndpoint);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      Serial.println("Failed to connect to server");
    }

    http.end();
  }
}
