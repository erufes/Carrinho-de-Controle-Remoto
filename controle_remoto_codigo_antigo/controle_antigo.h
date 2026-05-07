#include <WiFi.h>
#include <WebServer.h>

// WiFi
const char* ssid = "Teste";
const char* password = "12345678";

// Server
WebServer server(80);

// Motor pins
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14;

int motor2Pin1 = 33; 
int motor2Pin2 = 25; 
int enable2Pin = 32;

// PWM
const int freq = 30000;
const int resolution = 8;
int dutyCycle = 0;

// ===== Handlers =====
void handleRoot() {
  const char html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      html { font-family: Helvetica; text-align: center; }
      .button { background-color: #4CAF50; border: none; color: white; padding: 12px 28px; font-size: 26px; margin: 1px; }
      .button2 { background-color: #555555; }
    </style>

    <script>
      function moveForward(){ fetch('/forward'); }
      function moveLeft(){ fetch('/left'); }
      function stopRobot(){ fetch('/stop'); }
      function moveRight(){ fetch('/right'); }
      function moveReverse(){ fetch('/reverse'); }

      function updateMotorSpeed(pos){
        document.getElementById('motorSpeed').innerHTML = pos;
        fetch(`/speed?value=${pos}`);
      }
    </script>
  </head>

  <body>
    <h1>ESP32 Motor Control</h1>

    <button class="button" onclick="moveForward()">FORWARD</button><br><br>

    <button class="button" onclick="moveLeft()">LEFT</button>
    <button class="button button2" onclick="stopRobot()">STOP</button>
    <button class="button" onclick="moveRight()">RIGHT</button><br><br>

    <button class="button" onclick="moveReverse()">REVERSE</button>

    <p>Speed: <span id="motorSpeed">0</span></p>

    <input type="range" min="0" max="100" step="25"
           oninput="updateMotorSpeed(this.value)" value="0"/>
  </body>
  </html>)rawliteral";

  server.send(200, "text/html", html);
}

// ===== Motor actions =====
void forward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  server.send(200);
}

void left() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  server.send(200);
}

void stopRobot() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  server.send(200);
}

void right() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  server.send(200);
}

void reverse() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  server.send(200);
}

void speed() {
  if (server.hasArg("value")) {
    int value = server.arg("value").toInt();
    dutyCycle = map(value, 0, 100, 0, 255);

    ledcWrite(enable1Pin, dutyCycle);
    ledcWrite(enable2Pin, dutyCycle);
  }
  server.send(200);
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  ledcAttach(enable1Pin, freq, resolution);
  ledcAttach(enable2Pin, freq, resolution);

  ledcWrite(enable1Pin, 0);
  ledcWrite(enable2Pin, 0);

  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  // Routes
  server.on("/", handleRoot);
  server.on("/forward", forward);
  server.on("/left", left);
  server.on("/stop", stopRobot);
  server.on("/right", right);
  server.on("/reverse", reverse);
  server.on("/speed", speed);

  server.begin();
}

// ===== LOOP =====
void loop() {
  server.handleClient();
}