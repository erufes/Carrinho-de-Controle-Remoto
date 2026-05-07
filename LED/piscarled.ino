#include <WiFi.h>

const int led = 4;
const char* ssid = "Teste_ESP32";
const char* pass = "12345678";
bool ledState = false;

WiFiServer server(80);
IPAddress ip_server;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW); // começa ligado

  if (WiFi.softAP(ssid, pass)) {
    ip_server = WiFi.softAPIP();
    Serial.print("AP iniciado. IP: ");
    Serial.println(ip_server);
  } else {
    Serial.println("Falha ao iniciar o AP.");
  }

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available()) delay(1);

  String request = client.readStringUntil('\r');
  client.flush();
  Serial.println(request);

  if (request.indexOf("/on") != -1) {
    ledState = true;
    digitalWrite(led, LOW);
  } else if (request.indexOf("/off") != -1) {
    ledState = false;
    digitalWrite(led, HIGH);
  }

  String html = "HTTP/1.1 200 OK\r\n";
  html += "Content-type:text/html\r\n\r\n";
  html += "<html><body style='font-family:sans-serif;text-align:center;margin-top:50px'>";
  html += "<h2>Controle do LED</h2>";

  if (ledState) {
    html += "<p>Status: <strong>LIGADO</strong></p>";
    html += "<a href='/off'><button style='padding:20px 40px;font-size:18px;background:red;color:white;border:none;border-radius:8px'>Desligar</button></a>";
  } else {
    html += "<p>Status: <strong>DESLIGADO</strong></p>";
    html += "<a href='/on'><button style='padding:20px 40px;font-size:18px;background:green;color:white;border:none;border-radius:8px'>Ligar</button></a>";
  }

  html += "</body></html>";

  client.print(html);
  client.stop();
}