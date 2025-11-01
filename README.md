# 🌡️ Sistema de Monitoramento de Temperatura Corporal com Arduino e IoT
### Projeto Final – Objetos Inteligentes Conectados
**Autora:** Adriana Santos da Cruz  
**Instituição:** Universidade Presbiteriana Mackenzie  
**Ano:** 2025  
**ODS 3 – Saúde e Bem-Estar**

---

## 🧠 Descrição do Projeto
Este sistema realiza **medições contínuas de temperatura corporal**, enviando dados via **protocolo MQTT** para um **painel Node-RED Dashboard**, com **acionamento automático de LEDs e buzzer** em caso de febre (≥ 37,5 °C).

O projeto foi desenvolvido com **Arduino Uno**, **sensor LM35**, **módulo Wi-Fi ESP8266-01** e **Shield Multifunções HY-M302**, integrando hardware acessível, código aberto e comunicação em tempo real.

---

## ⚙️ Funcionalidades
- Medição contínua de temperatura corporal;
- Envio dos dados via MQTT (broker `test.mosquitto.org`);
- Exibição em tempo real no painel Node-RED;
- Acionamento de LED vermelho e buzzer quando há febre;
- LED verde ativo em temperatura normal;
- Registro gráfico de temperatura e tempo de resposta.

---

## 🧩 Componentes e Hardware

| Componente | Função | Conexões principais |
|-------------|--------|---------------------|
| Arduino Uno | Microcontrolador principal | Coordena sensores e atuadores |
| LM35 | Sensor de temperatura analógico | Vout → A0 / Vcc → 5V / GND → GND |
| ESP8266-01 | Comunicação MQTT | RX↔TX via divisor resistivo / CH_PD→3.3V |
| Shield HY-M302 | LED vermelho, LED verde e buzzer | D3, D4 e D5 |
| Node-RED + Mosquitto | Visualização e controle remoto | Painel Dashboard via MQTT |

📎 **Diagrama Fritzing:** [`/arduino/LM35_diagrama_fritzing.png`](arduino/LM35_diagrama_fritzing.png)  
📎 **Fluxo Node-RED:** [`/node-red/flow_projeto_adriana_cruz.json`](node-red/flow_projeto_adriana_cruz.json)

---

## 🖥️ Software e Protocolos
- **Arduino IDE 2.3.6**
- **Node-RED 3.x**
- **Broker MQTT:** `test.mosquitto.org`
- **Linguagem:** C/C++ (Arduino) e JavaScript (Node-RED)
- **Bibliotecas:** `PubSubClient.h`, `ESP8266WiFi.h`

---

## 🔄 Fluxograma do Funcionamento

```mermaid
flowchart TD
    A[Sensor LM35] -->|Lê temperatura| B[Arduino Uno]
    B -->|Converte e envia via Serial| C[ESP8266-01]
    C -->|Publica em tópico MQTT| D[Broker Mosquitto]
    D -->|Recebe no Node-RED| E[Dashboard]
    E -->|Verifica temperatura| F{Temp ≥ 37.5°C?}
    F -->|Sim| G[LED Vermelho + Buzzer]
    F -->|Não| H[LED Verde]
