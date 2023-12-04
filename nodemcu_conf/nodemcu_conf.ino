#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

/*
 Ganti placeholder dengan:
 - placeholder1 = Nama SSID Wi-Fi
 - placeholder2 = Kata sandi Wi-Fi
 - placeholder3 = Alamat IP atau nama host server yang digunakan di web_conf/src/webapi/server.js
 Pastikan bahwa NodeMCU dan server terhubung ke jaringan Wi-Fi yang sama.
*/
const char *ssid = "placeholder1"; 
const char *password = "placeholder2";
const char *serverAddress = "http://placeholder3:9000";

const int moist_pin = A0; // Pin sensor kelembaban tanah
const int vibr_pin = D0; // Pin sensor getaran
const int buzzerPin = D2; // Pin buzzer

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
  String level;

  level = checkLandslide(soilMoisture);
  
  Serial.print("Kelembaban Tanah: ");
  Serial.println(soilMoisture);
  Serial.print("Getaran: ");
  Serial.println(vibration);
  Serial.print("Level: ");
  Serial.println(level);
  
  sendToServer(soilMoisture, vibration, level);

  delay(5000);
}

void sendToServer(int soilMoisture, int vibration, String level) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    String data = "moist=" + String(soilMoisture) + "&vibr=" + String(vibration) + "&level=" + String(level);
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

String checkLandslide(int soilMoisture) {
  float moisturePercentage = map(soilMoisture, 1024, 0, 0, 100); // Konversi nilai sensor ke persen
  
  Serial.print("Persentase Kelembaban Tanah: ");
  Serial.print(moisturePercentage);
  Serial.println("%");
  
  if (moisturePercentage > 54) {
    // Kondisi bahaya
    tone(buzzerPin, 1000); // Nada frekuensi tinggi untuk peringatan bahaya
    return "bahaya";
  } else if (moisturePercentage >= 27 && moisturePercentage <= 54) {
    // Kondisi siaga
    noTone(buzzerPin); // Matikan buzzer
    return "siaga";
  } else {
    // Kondisi normal
    noTone(buzzerPin); // Matikan buzzer
    return "normal";
  }
}
