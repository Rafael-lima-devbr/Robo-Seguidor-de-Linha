int sensores[5] = { 0, 0, 0, 0, 0 };
int pinos_sensor[5] = { A1, A2, A3, A4, A5 };
int sensores_max[5] = { 0, 0, 0, 0, 0 };
int sensores_min[5] = { 1023, 1023, 1023, 1023, 1023 };
int sensor_normal[5] = { 0, 0, 0, 0, 0 };
int pesos[5] = { -4, -2, 0, 2, 4 };
int vel_base  = 70;
int motorE1 = 5;
int motorE2 = 6;
int motorD1 = 9;
int motorD2 = 10;
float integral = 0;
float kp = 30;
float ki = 0;
float kd = 100;
void setup() {
  pinMode(motorE1, OUTPUT);
  pinMode(motorE2, OUTPUT);
  pinMode(motorD1, OUTPUT);
  pinMode(motorD2, OUTPUT);
  Serial.begin(9600);
  Serial.println("Calibrando...");
  calibracao();
}
void loop() {
  float pos = line_pos();
  float pid = calculaPID(pos);
  MotorPID(pid);
} 
void readRaw() { // Leitura Bruta dos Sensores
  for (int i = 0; i < 5; i++) {
    sensores[i] = analogRead(pinos_sensor[i]);
  }
}  
void calibracao() {  // Calibaração de valores máximos e mínimos de leitura
  unsigned long timer1 = millis();
  while (millis() - timer1 < 5000) {
    readRaw();
    for (int i = 0; i < 5; i++) {
      if (sensores[i] > sensores_max[i]) sensores_max[i] = sensores[i];
      if (sensores[i] < sensores_min[i]) sensores_min[i] = sensores[i];
    }
  }
    for (int i = 0; i < 5; i++) {   
    if (sensores_max[i] - sensores_min[i] < 50) {
        sensores_min[i] -= 25;
        sensores_max[i] += 25;
      }
    }
}
void normal() { //Normaliza os valores lidos para uma faixa de 0 a 100
  readRaw();
  for (int i = 0; i < 5; i++) {
    sensor_normal[i] = map(sensores[i], sensores_min[i], sensores_max[i], 0, 100);
    if (sensor_normal[i] < 0) sensor_normal[i] = 0;
    else if (sensor_normal[i] > 100) sensor_normal[i] = 100;

    Serial.print(" | ");
    Serial.print(sensor_normal[i]);
  }
  Serial.println(" | ");
}
float line_pos() { //Gera a posição da linha em relação ao rôbo
  normal();
float pos = 0;
float soma = 0;
static float lastpos = 0;
float total = 0;
 for (int i = 0; i<5; i++) { 
  soma = soma + sensor_normal[i] * pesos[i];
  total = total + sensor_normal[i]; 
 }
  if (total != 0) {
    pos = soma / total;
    lastpos = pos;
  } else {
    pos = lastpos;
  }
  return pos;
}
float calculaPID(float pos) { //Calcula a correção do rôbo para o motor
  float erro = -pos;
  unsigned long now = millis();
  float dt = now - lastime;
  integral= integral + erro*dt;
    if (integral > 250) integral = 250; 
    if (integral < -250) integral = -250;
  float derivada = (erro - erroanterior)/dt;
  float saida = kp * erro + ki * integral + kd * derivada;
  static float erroanterior = erro;
  static unsigned long lastime = now;
  return saida;
}
void MotorPID(float pid) { //Realiza o movimento de correção calculado pelo PID
  int velE = vel_base + pid;
  int velD = vel_base - pid;
  if (velE > 255) velE = 255;
  if (velE < -255) velE = -255;
  if (velD > 255) velD = 255;
  if (velD < -255) velD = -255;
    if (velD >= 0) {
      analogWrite(motorD1, velD);
      analogWrite(motorD2, 0);
    } else {
      analogWrite(motorD1, 0);
      analogWrite(motorD2, -velD);
    } 
  if (velE >= 0) {
      analogWrite(motorE1, velE);
      analogWrite(motorE2, 0);
    } else {
      analogWrite(motorE1, 0);
      analogWrite(motorE2, -velE);
}
}
