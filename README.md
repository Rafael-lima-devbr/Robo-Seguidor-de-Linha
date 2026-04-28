# 🤖 Robô Seguidor de Linha com PID (PD)

## 📌 Descrição
Robô seguidor de linha utilizando **5 sensores analógicos** e controle **PD (Proporcional + Derivativo)** para correção de trajetória em tempo real.  
O sistema possui calibração automática, normalização dos sensores, cálculo da posição da linha e lógica especial para curvas bruscas e parada inteligente.

---

## ⚙️ Funcionamento

### 🟢 1. Calibração
- Executada por aproximadamente **5 segundos** ao iniciar
- Captura os valores **mínimos e máximos** de cada sensor

### 🔵 2. Leitura e Normalização
- Valores brutos convertidos para escala de **0 a 100**
- Garante consistência mesmo com variações do ambiente

### 🟣 3. Cálculo da Posição da Linha
- Média ponderada utilizando:
pesos = [-2, -1, 0, 1, 2]

- Se a linha for perdida, utiliza a **última posição válida**

### 🔴 4. Controle PD
Erro calculado como:
erro = setpoint (0) - posicao

Componentes:
- Proporcional (kp)
- Derivativo (kd, baseado no tempo dt)

Saída:
saida = kp * erro + kd * derivada

---

### 🟠 5. Controle dos Motores
- Velocidade base ajustada com saída do controle
- Limite mínimo de potência para evitar zona morta
- Controle de direção (frente e ré)

---

## 🧠 Lógica Especial

- **Curva forte à esquerda**
  - Ativada quando sensores da esquerda detectam linha forte

- **Curva forte à direita**
  - Ativada quando sensores da direita detectam linha forte

- **Parada inteligente**
  - Quando todos sensores detectam linha:
    - Aguarda curto período
    - Se perder o sensor central → robô para completamente

---

## 🔌 Hardware

- Arduino (Uno/Nano)
- 5 sensores IR analógicos
- Driver de motor (ex: L298N)
- 2 motores DC

---

## ⚙️ Parâmetros

### Controle (PD)
kp = 130  
kd = 2  

### Velocidade Base
velocidade_base = 170  

### Pesos dos Sensores
[-2, -1, 0, 1, 2]

---

## 🔍 Detalhes Técnicos

- Cálculo de tempo:
dt = (now - lasttime) / 1000.0

- Conversão analógico → digital:
sensor_normal >= 50 → 1  
sensor_normal < 50 → 0  

- Saída dos motores:
velocidade entre -255 e 255  

- Zona mínima de potência:
valores entre -120 e 120 são ajustados para evitar baixa resposta  

- Tratamento de perda de linha:
usa última posição válida ao invés de zerar

---

## ▶️ Fluxo do Programa

setup()
 ├── inicialização
 ├── configuração de pinos
 └── calibração

loop()
 └── controle()
      ├── leitura digital
      ├── lógica especial
      └── PD + motores

---

## 📜 Licença
MIT License
