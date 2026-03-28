# 🤖 Line Follower Robot with PID

## 🇧🇷 Português

### 📌 Descrição

Robô seguidor de linha utilizando 5 sensores analógicos e controle PID para correção de trajetória em tempo real.

---

### ⚙️ Funcionamento

* Calibração automática dos sensores (5s)
* Normalização dos valores (0–100)
* Cálculo da posição da linha (média ponderada)
* Correção via PID
* Ajuste dinâmico dos motores

---

### 🔌 Hardware

* Arduino (Uno/Nano)
* 5 sensores IR
* Driver de motor (ex: L298N)
* 2 motores DC

---

### 🎛️ Parâmetros PID

* kp = 30
* ki = 0
* kd = 100

---

### ⚠️ Observação

A função de PID possui erro de inicialização em variáveis (`lastime` e `erroanterior`) e deve ser corrigida.

---

## 🇺🇸 English

### 📌 Description

Line follower robot using 5 analog sensors and PID control for real-time trajectory correction.

---

### ⚙️ How it works

* Automatic sensor calibration (5s)
* Value normalization (0–100)
* Line position calculation (weighted average)
* PID correction
* Dynamic motor adjustment

---

### 🔌 Hardware

* Arduino (Uno/Nano)
* 5 IR sensors
* Motor driver (e.g., L298N)
* 2 DC motors

---

### 🎛️ PID Parameters

* kp = 30
* ki = 0
* kd = 100

---

### ⚠️ Note

The PID function has an initialization issue with variables (`lastime` and `erroanterior`) and should be fixed.

---

## 📜 License

MIT License (recommended)
