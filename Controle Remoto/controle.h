#include <WiFi.h>        // biblioteca do Wi-Fi da ESP32
#include <WebServer.h>   // biblioteca do servidor web

const char* ssid = "Vasco";
const char* password = "12345678";
WebServer server(80);

// motor esquerdo

// pino IN1 da ponte H
int motor1Pin1 = 25;
// pino IN2 da ponte H
int motor1Pin2 = 26;

// motor direito
// pino IN3 da ponte H
int motor2Pin1 = 27;
// pino IN4 da ponte H
int motor2Pin2 = 14;

// guarda o tempo do último comando recebido
unsigned long ultimoComando = 0;

// tempo máximo sem comando antes de parar
const unsigned long timeoutParada = 300;


//motores
void pararMotores() {

  // desliga motor esquerdo
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  // desliga motor direito
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

// função que envia a página web
void handleRoot() {

  // cria o HTML da página
  String html = R"rawliteral(

<!DOCTYPE html>
<html lang="pt-BR">

<head>
<meta charset="UTF-8">

<!-- configuração de tela para celular -->
<meta name="viewport"
content="width=device-width,
initial-scale=1,
maximum-scale=1,
user-scalable=no">


<title>Projeto Carro 1</title>

<style>

/* reseta margens padrão */
*{
  margin:0;
  padding:0;
  box-sizing:border-box;
}

/* estilo do fundo */
body{

  background:linear-gradient(135deg,#0f172a,#1e293b);

  min-height:100vh;

  display:flex;
  justify-content:center;
  align-items:center;

  font-family:Arial, Helvetica, sans-serif;

  /* impede selecionar texto  MUITO IMPORTANTE*/ 
  user-select:none;
  -webkit-user-select:none;

  overflow:hidden;
}

/* caixa principal */
.container{

  width:95%;
  max-width:420px;

  background:rgba(255,255,255,0.08);

  backdrop-filter:blur(15px);

  border:1px solid rgba(255,255,255,0.1);

  border-radius:28px;

  padding:30px 20px;

  box-shadow:
  0 0 30px rgba(0,0,0,0.35);

  text-align:center;
}

h1{

  color:white;

  margin-bottom:35px;

  font-size:2rem;
}

/* linha dos botões */
.row{
  display:flex;
  justify-content:center;
  gap:16px;
  margin:18px 0;
}

/* estilo geral dos botões */
button{

  width:110px;
  height:110px;

  border:none;
  border-radius:24px;

  font-size:1rem;
  font-weight:bold;

  color:white;

  cursor:pointer;

  transition:0.15s;

  touch-action:none;

  user-select:none;
  -webkit-user-select:none;

  box-shadow:
  0 8px 20px rgba(0,0,0,0.35);
}

/* animação ao apertar */
button:active{
  transform:scale(0.92);
}

/* botão frente */
.forward{
  background:linear-gradient(145deg,#22c55e,#16a34a);
}

/* botão ré */
.reverse{
  background:linear-gradient(145deg,#f97316,#ea580c);
}

/* botões esquerda e direita */
.left,
.right{
  background:linear-gradient(145deg,#3b82f6,#2563eb);
}

</style>

<script>

// variável do intervalo
let interval = null;

// guarda qual comando está ativo
let comandoAtual = null;

// função que envia comandos pro ESP32
function enviar(cmd){

  // faz uma requisição HTTP
  fetch(cmd).catch(() => {});
}

// começa um movimento
function iniciar(cmd){

  // evita criar vários intervals iguais
  if(comandoAtual === cmd){
    return;
  }

  // para o anterior
  parar();

  // salva o comando atual
  comandoAtual = cmd;

  // envia o comando imediatamente
  enviar(cmd);

  // continua enviando o comando
  interval = setInterval(() => {

    enviar(cmd);

  }, 100);
}

// para o robô
function parar(){

  // se existir intervalo
  if(interval){

    // cancela o intervalo
    clearInterval(interval);

    interval = null;
  }

  // limpa o comando atual
  comandoAtual = null;

  // envia comando de stop
  enviar('/stop');
}

// quando solta o mouse(por algum motivo que eu não sei pq não tá funcionando a parte do computador)
document.addEventListener('mouseup', parar);

// quando solta o dedo
document.addEventListener('touchend', parar);

// quando troca de aba ou perde foco
window.addEventListener('blur', parar);

</script>

</head>

<body>

<div class="container">

  <h1>Projeto Carro 1</h1>

  <!-- botão frente -->
  <div class="row">

    <button
      class="forward"

      onpointerdown="iniciar('/forward')"

      onpointerup="parar()"

      onpointerleave="parar()"

      onpointercancel="parar()">

      FRENTE

    </button>

  </div>

  <!-- linha esquerda e direita -->
  <div class="row">

    <!-- esquerda -->
    <button
      class="left"

      onpointerdown="iniciar('/left')"

      onpointerup="parar()"

      onpointerleave="parar()"

      onpointercancel="parar()">

      ESQ

    </button>

    <!-- direita -->
    <button
      class="right"

      onpointerdown="iniciar('/right')"

      onpointerup="parar()"

      onpointerleave="parar()"

      onpointercancel="parar()">

      DIR

    </button>

  </div>

  <!-- botão ré -->
  <div class="row">

    <button
      class="reverse"

      onpointerdown="iniciar('/reverse')"

      onpointerup="parar()"

      onpointerleave="parar()"

      onpointercancel="parar()">

      RÉ

    </button>

  </div>

</div>

</body>
</html>

)rawliteral";

  // envia o HTML pro navegador
  server.send(200, "text/html", html);
}

// movimento frente
void frente() {

  // salva horário do comando
  ultimoComando = millis();

  Serial.println("FRENTE");

  // motor esquerdo frente
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  // motor direito frente
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

  server.send(200, "text/plain", "ok");
}

// movimento ré
void re() {

  ultimoComando = millis();

  Serial.println("RE");

  // motor esquerdo ré
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  // motor direito ré
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  server.send(200, "text/plain", "ok");
}

// virar esquerda
void esquerda() {

  ultimoComando = millis();

  Serial.println("ESQUERDA");

  // esquerda pra trás
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  // direita pra frente
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

  server.send(200, "text/plain", "ok");
}

// virar direita
void direita() {

  ultimoComando = millis();

  Serial.println("DIREITA");

  // esquerda pra frente
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  // direita pra trás
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  server.send(200, "text/plain", "ok");
}

// parar robô
void parar() {

  Serial.println("PARAR");

  pararMotores();

  server.send(200, "text/plain", "ok");
}

void setup() {

  // inicia serial
  Serial.begin(115200);

  // define pinos como saída
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // garante que começa parado
  pararMotores();

  // cria rede Wi-Fi
  WiFi.softAP(ssid, password);

  Serial.println();

  Serial.println("WiFi iniciado!");

  // mostra IP do ESP32
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.softAPIP());

  // página principal
  server.on("/", handleRoot);

  // comandos do robô
  server.on("/forward", frente);

  server.on("/reverse", re);

  server.on("/left", esquerda);

  server.on("/right", direita);

  server.on("/stop", parar);

  // inicia servidor
  server.begin();

  Serial.println("Servidor iniciado!");
}

void loop() {

  // verifica clientes conectados
  server.handleClient();

  // se ficar sem comando por muito tempo
  if (millis() - ultimoComando > timeoutParada) {

    // para os motores
    pararMotores();
  }
}