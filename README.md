# 🚗 Carrinho-de-Controle-Remoto

Projeto do Trainee 2026/1 — Projeto 1  
Robô seguidor de linha com controle remoto via Wi-Fi utilizando ESP32.

---

# 📖 Descrição

O projeto consiste no desenvolvimento de um carro robótico capaz de operar em dois modos:

- 🔲 Modo seguidor de linha
- 📱 Controle remoto via navegador

O sistema utiliza uma ESP32 como unidade principal de controle, responsável pela comunicação Wi-Fi, gerenciamento dos motores e execução da lógica do robô.

Além do funcionamento autônomo, o veículo também pode ser controlado manualmente através de uma interface web moderna hospedada diretamente na ESP32.

---

# ⚡ Funcionalidades

## 🔲 Seguimento de Linha

O robô utiliza sensores infravermelhos para detectar e seguir trajetos demarcados no chão de forma automática.

### Tecnologias previstas
- ESP32
- Sensores infravermelhos
- Ponte H
- Motores DC

---

## 📱 Controle Remoto via Wi-Fi

A ESP32 cria sua própria rede Wi-Fi e disponibiliza uma interface web para controle do veículo em tempo real.

### Funcionalidades
- Movimento para frente
- Ré
- Curva para esquerda
- Curva para direita
- Parada automática de segurança
- Compatibilidade com celular e computador

### Tecnologias utilizadas
- ESP32
- Wi-Fi Access Point
- WebServer
- HTML/CSS/JavaScript

---

# 🧠 Diferenciais do Projeto

- Interface web embarcada na ESP32
- Controle sem necessidade de internet
- Sistema de timeout para segurança
- Baixa latência nos comandos
- Compatibilidade mobile
- Estrutura preparada para futuras expansões

---

# 🛠️ Estrutura do Sistema

O projeto é dividido em três partes principais:

## 🔌 Controle dos Motores
Responsável pela movimentação do robô através da ponte H.

## 🌐 Servidor Web
Gerencia a interface gráfica e os comandos enviados pelo navegador.

## 🤖 Lógica do Robô
Responsável pelo comportamento do veículo em cada modo de operação.

---

# 📡 Rede Wi-Fi

Ao iniciar, a ESP32 cria automaticamente uma rede Wi-Fi.

Exemplo:
```txt
SSID: Teste-ESP32
Senha: 12345678

C++
ESP32
Arduino Framework
HTML5
CSS3
JavaScript
Wi-Fi Embedded Systems
📌 Objetivo

O projeto tem como objetivo aplicar conceitos de:

Robótica
Sistemas embarcados
Programação
Automação
Redes
Desenvolvimento web

integrando hardware e software em uma aplicação prática.