// Dual DC Motor Shield V2 — STM32 Nucleo F446 — Arduino IDE

// Pins canal A (moteurs GAUCHE)
#define IN1_1  3    // direction gauche
#define IN1_2  5    // PWM vitesse gauche

// Pins canal B (moteurs DROIT)
#define IN2_1  6    // direction droite
#define IN2_2  9    // PWM vitesse droite

void setup() {
  pinMode(IN1_1, OUTPUT);
  pinMode(IN1_2, OUTPUT);
  pinMode(IN2_1, OUTPUT);
  pinMode(IN2_2, OUTPUT);
  stopMotors();
}

// ---- Fonctions de base ----

void setLeft(int direction, int speed) {
  // direction : 1=avant, -1=arriere, 0=stop
  // speed : 0 à 255
  if (direction == 1) {
    digitalWrite(IN1_1, LOW);
    analogWrite(IN1_2, speed);
  } else if (direction == -1) {
    digitalWrite(IN1_1, HIGH);
    analogWrite(IN1_2, speed);
  } else {
    digitalWrite(IN1_1, LOW);
    analogWrite(IN1_2, 0);
  }
}

void setRight(int direction, int speed) {
  if (direction == 1) {
    digitalWrite(IN2_1, LOW);
    analogWrite(IN2_2, speed);
  } else if (direction == -1) {
    digitalWrite(IN2_1, HIGH);
    analogWrite(IN2_2, speed);
  } else {
    digitalWrite(IN2_1, LOW);
    analogWrite(IN2_2, 0);
  }
}

// ---- Commandes haut niveau ----

void stopMotors() {
  setLeft(0, 0);
  setRight(0, 0);
}

void forward(int speed) {
  setLeft(1, speed);
  setRight(1, speed);
}

void backward(int speed) {
  setLeft(-1, speed);
  setRight(-1, speed);
}

void turnLeft(int speed) {
  setLeft(-1, speed);
  setRight(1, speed);
}

void turnRight(int speed) {
  setLeft(1, speed);
  setRight(-1, speed);
}

// ---- Test séquence ----

void loop() {
  forward(180);     // 70% vitesse (~180/255)
  
}