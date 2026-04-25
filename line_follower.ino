const int motorE1 = 5;
const int motorE2 = 6;
const int motorD1 = 10;
const int motorD2 = 9;
int sensor_leitura[5] = {A0, A1, A2, A3, A4};
float sensor_bruto[5] {0,0,0,0,0};
float sensor_max[5]= {0,0,0,0,0};
float sensor_min[5] = { 1023, 1023, 1023, 1023, 1023};
float sensor_normal[5] = {0,0,0,0,0};
int sensor_digital[5] = {0,0,0,0,0};
int pesos[5] = {-2,-1,0,1,2};
float integral = 0;
float kp = 140;
float ki = 0;
float kd = 1.1;
int velocidade_base = 200;
int led = 13;
void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(motorE1, OUTPUT);
  pinMode(motorE2, OUTPUT);
  pinMode(motorD1, OUTPUT);
  pinMode(motorD2, OUTPUT);
  Serial.println("Calibrando...");
  calibracao();
}

void loop(){
  float pos = posicao_linha();
  controle(pos);
}

void readraw(){
  for (int i = 0; i<5; i++){
    sensor_bruto[i] = analogRead(sensor_leitura[i]);
  }
}

void calibracao() {
  unsigned long timer = millis();
  while (millis() - timer < 5000) {
    readraw();
    for (int i = 0; i<5; i++){
    if (sensor_max[i] < sensor_bruto[i]) sensor_max[i] = sensor_bruto[i];
    if (sensor_min[i] > sensor_bruto[i]) sensor_min[i] = sensor_bruto[i];
    }
  }
}

void normalizacao() {
  readraw();
  for (int i = 0; i<5; i++){
  sensor_normal[i] = constrain(map(sensor_bruto[i], sensor_min[i], sensor_max[i], 0, 100), 0, 100);
  //Serial.print(" | ");
  //Serial.print(sensor_normal[i]);
  //.print(" | ");
  }
  //Serial.println();
}

float posicao_linha() {
  normalizacao();
  float posicao = 0;
  static float lastposicao = 0;
  float total = 0;
  float soma = 0;
  for (int i = 0; i<5; i++) {
    soma += sensor_normal[i] * pesos[i];
    total += sensor_normal[i];
  }  
  if (total > 10) {
      posicao = soma/total;
      lastposicao = posicao;
  } else {
    posicao = lastposicao;
}
return posicao;
}

float PID(float posicao){
  float setpoint = 0;
  static float erroanterior = 0;
  static unsigned long lastime = 0;
  float erro = setpoint-posicao;
  unsigned long now= millis();
  float dt = (now-lastime)/1000.0;
  integral += erro*dt;
  integral = constrain(integral, -250 , 250);
  float derivada = (erro-erroanterior)/dt;
  float saida = kp*erro + ki*integral + kd*derivada;
  erroanterior = erro;
  lastime = now;
  return saida;
}

void motor(float pid) {
  int velocidade_esquerda = constrain(velocidade_base + pid, -255, 255);
  int velocidade_direita = constrain(velocidade_base - pid, -255, 255);
  if (velocidade_direita > -100 && velocidade_direita <0) velocidade_direita = -100;
  if (velocidade_direita <100 && velocidade_direita>0) velocidade_direita = 100;
  if (velocidade_esquerda> -100 && velocidade_esquerda <0) velocidade_esquerda = -100;
  if (velocidade_esquerda <100 &&  velocidade_esquerda >0) velocidade_esquerda = 100;
  
  if (velocidade_direita >= 0) {
      analogWrite(motorD1, velocidade_direita);
      analogWrite(motorD2, 0);
  } else {
      analogWrite(motorD1, 0);
      analogWrite(motorD2, -velocidade_direita);
    } 
  if (velocidade_esquerda >= 0) {
      analogWrite(motorE1, velocidade_esquerda);
      analogWrite(motorE2, 0);
  } else {
      analogWrite(motorE1, 0);
      analogWrite(motorE2, -velocidade_esquerda);
    }  
}

void digital(){
  for (int i = 0; i<5; i++) {
  if (sensor_normal[i] >= 50) {
  sensor_digital[i] = 1;
  } else sensor_digital[i] = 0; 
  //Serial.print(" | ");
  //Serial.print(sensor_digital[i]);
  //Serial.print(" | "); 
  }
}

void controle(float pos){
  digital();
    if (sensor_digital[0] + sensor_digital[1] >= 2 && sensor_normal[4] == 0){
      digitalWrite(led, 1);
        analogWrite(motorD2, velocidade_base);
        analogWrite(motorE1, velocidade_base);
        analogWrite(motorE2, 0);
        analogWrite(motorD1, 0);
        delay(200);
        digitalWrite(led, 0);
    } else if (sensor_digital[4] + sensor_digital[3] >=2 && sensor_normal[0] == 0){
      digitalWrite(led, 1);
        analogWrite(motorD1, velocidade_base);
        analogWrite(motorE2, velocidade_base);
        analogWrite(motorE1, 0);
        analogWrite(motorD2, 0);
        delay(200);
        digitalWrite(led, 0);
    } else if (sensor_digital[0] && sensor_digital[1] && sensor_digital[2] && sensor_digital[3] && sensor_digital[4]) {
        analogWrite(motorD1, 0);
        analogWrite(motorE2, 0);
        analogWrite(motorE1, 0);
        analogWrite(motorD2, 0);
        delay(7000);
    }
    else{
      float pid = PID(pos);
      motor(pid);
    }
}
