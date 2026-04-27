#include <WiFi.h>

// Tvé údaje k Wi-Fi sítě
const char* ssid = "AP_MLCAKOVA";
const char* password = "MlcakovaAP120";

// Nastavení pinu pro LED (obvykle GPIO 2)
const int ledPin = 2;

// Proměnná pro uchování aktuálního stavu LED
bool isLedOn = false; 

WiFiServer server(80);

// Náš HTML kód s připravenými značkami %LED_CLASS% a %LED_TEXT%
String baseHtml = R"=====(
<!DOCTYPE html>
<html lang="cs">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 | Ovládání</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #e9ecef;
            color: #333;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }
        .card {
            background-color: #ffffff;
            padding: 40px 30px;
            border-radius: 12px;
            box-shadow: 0 8px 20px rgba(0, 0, 0, 0.1);
            text-align: center;
            width: 90%;
            max-width: 400px;
        }
        h2 { margin-top: 0; margin-bottom: 25px; color: #2c3e50; }
        
        /* Nový design pro zobrazení stavu */
        .status-box {
            display: flex;
            align-items: center;
            justify-content: center;
            background-color: #f8f9fa;
            padding: 15px;
            border-radius: 8px;
            border: 1px solid #dee2e6;
            margin-bottom: 25px;
            font-size: 18px;
        }
        .led-indicator {
            width: 16px;
            height: 16px;
            border-radius: 50%;
            margin-right: 12px;
            transition: all 0.3s ease;
        }
        /* Barvy a záře pro stavy indikátoru */
        .led-on { background-color: #2ecc71; box-shadow: 0 0 12px #2ecc71; }
        .led-off { background-color: #e74c3c; box-shadow: 0 0 12px #e74c3c; }

        .btn {
            display: block; text-decoration: none; font-size: 22px;
            font-weight: bold; padding: 15px 30px; margin: 15px 0;
            border-radius: 8px; color: white; transition: all 0.3s ease;
        }
        .btn-on { background-color: #2ecc71; box-shadow: 0 4px 6px rgba(46, 204, 113, 0.3); }
        .btn-on:hover { background-color: #27ae60; transform: translateY(-2px); box-shadow: 0 6px 12px rgba(46, 204, 113, 0.4); }
        .btn-off { background-color: #e74c3c; box-shadow: 0 4px 6px rgba(231, 76, 60, 0.3); }
        .btn-off:hover { background-color: #c0392b; transform: translateY(-2px); box-shadow: 0 6px 12px rgba(231, 76, 60, 0.4); }
    </style>
</head>
<body>
    <div class="card">
        <h2>Ovládání LED na ESP32</h2>
        
        <div class="status-box">
            <div class="led-indicator %LED_CLASS%"></div>
            <span>Stav: <strong>%LED_TEXT%</strong></span>
        </div>

        <a href="/LED=ON" class="btn btn-on">Zapnout</a>
        <a href="/LED=OFF" class="btn btn-off">Vypnout</a>
    </div>
</body>
</html>
)=====";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  isLedOn = false; // Výchozí stav je vypnuto

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

    // Kontrola požadavku a změna skutečné LED i naší proměnné
    if (request.indexOf("/LED=ON") != -1) {
      digitalWrite(ledPin, HIGH);
      isLedOn = true;
    } else if (request.indexOf("/LED=OFF") != -1) {
      digitalWrite(ledPin, LOW);
      isLedOn = false;
    }

    // Příprava stránky k odeslání
    String pageToSend = baseHtml; // Vytvoříme kopii našeho HTML kódu
    
    // Nahrazení značek podle aktuálního stavu
    if (isLedOn) {
      pageToSend.replace("%LED_CLASS%", "led-on");
      pageToSend.replace("%LED_TEXT%", "ZAPNUTO");
    } else {
      pageToSend.replace("%LED_CLASS%", "led-off");
      pageToSend.replace("%LED_TEXT%", "VYPNUTO");
    }

    // Send HTTP response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    
    // Odeslání dynamicky upraveného HTML
    client.print(pageToSend);
    client.println();

    client.stop();
    Serial.println("Client disconnected");
  }
}