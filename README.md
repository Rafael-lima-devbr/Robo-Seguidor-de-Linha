# 🤖 Robô Seguidor de Linha com PID

## 📌 Descrição
Robô seguidor de linha utilizando **5 sensores analógicos** e controle **PID** para correção de trajetória em tempo real.  
O sistema implementa calibração automática, normalização dos sensores, cálculo da posição da linha e lógica especial para curvas e parada.

---

## ⚙️ Funcionamento

### 🟢 1. Calibração
- Executada por aproximadamente **5 segundos** ao iniciar
- Captura os valores **mínimos e máximos** de cada sensor

### 🔵 2. Leitura e Normalização
- Valores brutos são convertidos para uma escala de **0 a 100**
- Garante consistência entre os sensores

### 🟣 3. Cálculo da Posição da Linha
- Média ponderada utilizando:
pesos = [-2, -1, 0, 1, 2];
- Se a linha for perdida, o sistema utiliza a **última posição válida**

### 🔴 4. Controle PID
Erro calculado como:
erro = setpoint (0) - posicao;

Componentes:
- Proporcional  
- Integral (com limite para evitar windup)  
- Derivativo (baseado no tempo dt)  

### 🟠 5. Controle dos Motores
- Velocidade base ajustada com saída do PID  
- Limite mínimo para evitar zona morta do motor  
- Controle de direção (frente e ré)  

---

## 🧠 Lógica Especial

- Curva forte à esquerda  
  Quando sensores da esquerda detectam linha forte  

- Curva forte à direita  
  Quando sensores da direita detectam linha forte  

- Parada total  
  Quando todos os sensores detectam linha simultaneamente  

---

## 🔌 Hardware

- Arduino (Uno/Nano)  
- 5 sensores IR analógicos  
- Driver de motor (ex: L298N)  
- 2 motores DC  
- LED no pino 13  

---

## ⚙️ Parâmetros

### PID
kp = 140  
ki = 0  
kd = 1.1  

### Velocidade Base
velocidade_base = 200  

### Pesos
[-2, -1, 0, 1, 2]

---

## 🔍 Detalhes Técnicos

- Anti-windup no termo integral:
integral = constrain(integral, -250, 250);

- Cálculo de tempo:
dt = (now - lasttime) / 1000.0;

- Conversão para digital:
sensor_normal >= 50 → 1  
sensor_normal < 50 → 0  

- Saída dos motores:
velocidade entre -255 e 255  

- Zona mínima:
valores entre -100 e 100 são ajustados para evitar baixa potência  

---

## ▶️ Fluxo do Programa

setup()
 ├── inicialização
 ├── configuração de pinos
 └── calibração

loop()
 ├── cálculo da posição
 └── controle
      ├── lógica especial
      └── PID + motores

---

## 📜 Licença
MIT License
