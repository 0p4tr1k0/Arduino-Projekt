#include <WiFi.h>

// Replace with your Wi-Fi credentials
const char* ssid = "AP_MLCAKOVA";
const char* password = "MlcakovaAP120";

// Set LED pin (usually GPIO 2)
const int ledPin = 2;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Control LED based on request
    if (request.indexOf("/LED=ON") != -1) {
      digitalWrite(ledPin, HIGH);
    } else if (request.indexOf("/LED=OFF") != -1) {
      digitalWrite(ledPin, LOW);
    }

    // Send simple HTML response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

   client.println("<head><meta charset=\"UTF-8\"></head>");
    client.println("<body style='text-align:center; background-color: #F5F5DC; color: black;'>");
    client.println("<h2 style='font-family: verdana;'>Ovládání led světla na ESP32</h2>");
    client.println("<a href=\"/LED=ON\"><button style='background-color: #04AA6D; color: white; font-size: 30px; padding: 12px; border: none; border-radius: 5px; cursor: pointer;'>Zapnout</button></a>");
    client.println("&nbsp;");
    client.println("<a href=\"/LED=OFF\"><button style='background-color: #f44336; color: white; font-size: 30px; padding: 12px; border: none; border-radius: 5px; cursor: pointer;'>Vypnout</button></a>");
    client.println("</body></html>");

    client.stop();
    Serial.println("Client disconnected");
  }
}
