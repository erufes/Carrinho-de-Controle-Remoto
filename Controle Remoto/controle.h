/*
  ESP32 Wi-Fi Car Robot + Access Point Web Server
  Junta:
  - Controle do robô via navegador
  - ESP32 criando a própria rede Wi-Fi
  - Controle de velocidade
  - Interface web
*/

#include <WiFi.h>
#include <WebServer.h>
#include "site.h"

// =====================
// ACCESS POINT
// =====================
const char* ssid = "Vasco";
const char* password = "12345678";

// =====================
// WEB SERVER
// =====================
WebServer server(80);

// =====================
// MOTOR PINS
// =====================

// Motor 1
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

// Motor 2
int motor2Pin1 = 33;
int motor2Pin2 = 25;
int enable2Pin = 32;

// =====================
// PWM
// =====================
const int freq = 30000;
const int resolution = 8;

int dutyCycle = 0;
String valueString = "0";

// =====================
// HTML PAGE
// =====================
void handleRoot() {

  const char html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html>

  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <style>
      body {
        font-family: Arial;
        text-align: center;
        margin-top: 30px;
        background: #f2f2f2;
      }

      h1 {
        color: #333;
      }

      .button {
        background-color: #4CAF50;
        border: none;
        color: white;
        padding: 18px 35px;
        font-size: 22px;
        margin: 6px;
        border-radius: 10px;
        cursor: pointer;
      }

      .stop {
        background-color: red;
      }

      input[type=range] {
        width: 250px;
      }
    </style>

    <script>
      function moveForward() {
        fetch('/forward');
      }

      function moveLeft() {
        fetch('/left');
      }

      function moveRight() {
        fetch('/right');
      }

      function moveReverse() {
        fetch('/reverse');
      }

      function stopRobot() {
        fetch('/stop');
      }

      function updateMotorSpeed(pos) {
        document.getElementById("speedValue").innerHTML = pos;
        fetch("/speed?value=" + pos);
      }
    </script>
  </head>

  <body>

    <h1>ESP32 ROBOT CONTROL</h1>

    <p>
      <button class="button" onclick="moveForward()">FRENTE</button>
    </p>

    <p>
      <button class="button" onclick="moveLeft()">ESQUERDA</button>

      <button class="button stop" onclick="stopRobot()">PARAR</button>

      <button class="button" onclick="moveRight()">DIREITA</button>
    </p>

    <p>
      <button class="button" onclick="moveReverse()">RE</button>
    </p>

    <h3>Velocidade: <span id="speedValue">0</span></h3>

    <input type="range"
           min="0"
           max="100"
           step="25"
           value="0"
           oninput="updateMotorSpeed(this.value)">

  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

// =====================
// MOVEMENTS
// =====================

void handleForward() {

  Serial.println("FORWARD");

  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

  server.send(200);
}

void handleLeft() {

  Serial.println("LEFT");

  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

  server.send(200);
}

void handleRight() {

  Serial.println("RIGHT");

  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);

  server.send(200);
}

void handleReverse() {

  Serial.println("REVERSE");

  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  server.send(200);
}

void handleStop() {

  Serial.println("STOP");

  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);

  server.send(200);
}

// =====================
// SPEED CONTROL
// =====================

void handleSpeed() {

  if (server.hasArg("value")) {

    valueString = server.arg("value");
    int value = valueString.toInt();

    if (value == 0) {

      ledcWrite(enable1Pin, 0);
      ledcWrite(enable2Pin, 0);

      handleStop();

    } else {

      dutyCycle = map(value, 25, 100, 180, 255);

      ledcWrite(enable1Pin, dutyCycle);
      ledcWrite(enable2Pin, dutyCycle);

      Serial.print("Velocidade: ");
      Serial.println(value);
    }
  }

  server.send(200);
}

// =====================
// SETUP
// =====================

void setup() {

  Serial.begin(115200);

  // MOTOR OUTPUTS
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // PWM
  ledcAttach(enable1Pin, freq, resolution);
  ledcAttach(enable2Pin, freq, resolution);

  ledcWrite(enable1Pin, 0);
  ledcWrite(enable2Pin, 0);

  // =====================
  // CREATE ACCESS POINT
  // =====================

  Serial.println();
  Serial.println("Iniciando Access Point...");

  if (WiFi.softAP(ssid, password)) {

    Serial.println("AP iniciado com sucesso!");
    Serial.print("IP do ESP32: ");
    Serial.println(WiFi.softAPIP());

  } else {

    Serial.println("Erro ao iniciar AP");
  }

  // =====================
  // ROUTES
  // =====================

  server.on("/", handleRoot);

  server.on("/forward", handleForward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/reverse", handleReverse);
  server.on("/stop", handleStop);

  server.on("/speed", handleSpeed);

  // START SERVER
  server.begin();

  Serial.println("Servidor iniciado!");
}

// =====================
// LOOP
// =====================

void loop() {

  server.handleClient();
}