// Definição dos pinos dos Sensores (LDRs)
const int pinoLDR1 = A0;
const int pinoLDR2 = A1;
const int pinoLDR3 = A2;

// Definição dos pinos dos LEDs
const int pinoLED_GREEN_1 = 50;
const int pinoLED_GREEN_2 = 52;
const int pinoLED_RED = 53;

const int pinoRGB_Red   = 4;
const int pinoRGB_Green = 3;
const int pinoRGB_Blue  = 2;

// Limiar de leitura (ajuste conforme a luz do ambiente: quanto maior, mais escuro ou mais luz necessária)
// Lendo de 0 a 1023 no conversor analógico
const int LIMIAR_LUZ = 600;

void setup() {
  // Inicializa a comunicação serial para monitoramento (opcional)
  Serial.begin(9600);

  // Configura os pinos dos LEDs como saída
  pinMode(pinoLED_GREEN_1, OUTPUT);
  pinMode(pinoLED_GREEN_2, OUTPUT);
  pinMode(pinoLED_RED, OUTPUT);

  pinMode(pinoRGB_Red, OUTPUT);
  pinMode(pinoRGB_Green, OUTPUT);
  pinMode(pinoRGB_Blue, OUTPUT);

  // Garante que todos os LEDs começam apagados
  digitalWrite(pinoLED_GREEN_1, HIGH);
  digitalWrite(pinoLED_GREEN_2, LOW);
  digitalWrite(pinoLED_RED, LOW);

  analogWrite(pinoRGB_Red, 0);
  analogWrite(pinoRGB_Green, 0);
  analogWrite(pinoRGB_Blue, 0);
}

void loop() {
  int valorLDR1 = analogRead(pinoLDR1);
  int valorLDR2 = analogRead(pinoLDR2);
  int valorLDR3 = analogRead(pinoLDR3);

  if (valorLDR3 < LIMIAR_LUZ) {
    digitalWrite(pinoLED_GREEN_1, HIGH);
    digitalWrite(pinoLED_GREEN_2, HIGH);
    digitalWrite(pinoLED_RED, HIGH);

    analogWrite(pinoRGB_Red, 255);
    analogWrite(pinoRGB_Green, 150);
    analogWrite(pinoRGB_Blue, 0);
    delay(300);
  } else if (valorLDR2 < LIMIAR_LUZ) {
    digitalWrite(pinoLED_GREEN_1, HIGH);
    digitalWrite(pinoLED_GREEN_2, HIGH);
    digitalWrite(pinoLED_RED, LOW);

    analogWrite(pinoRGB_Red, 255);
    analogWrite(pinoRGB_Green, 150);
    analogWrite(pinoRGB_Blue, 0);
    delay(300);
  } else if (valorLDR1 < LIMIAR_LUZ) {
    digitalWrite(pinoLED_GREEN_1, HIGH);
    digitalWrite(pinoLED_GREEN_2, HIGH);
    digitalWrite(pinoLED_RED, LOW);

    analogWrite(pinoRGB_Red, 0);
    analogWrite(pinoRGB_Green, 0);
    analogWrite(pinoRGB_Blue, 0);
    delay(300);
  } else {
    digitalWrite(pinoLED_GREEN_1, HIGH);
    digitalWrite(pinoLED_GREEN_2, LOW);
    digitalWrite(pinoLED_RED, LOW);

    analogWrite(pinoRGB_Red, 0);
    analogWrite(pinoRGB_Green, 0);
    analogWrite(pinoRGB_Blue, 0);
    delay(300);
  }
}