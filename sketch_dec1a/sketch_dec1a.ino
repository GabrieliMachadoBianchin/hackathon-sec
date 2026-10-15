#include <SPI.h>
#include <MFRC522.h>

// Pinos do RFID RC522 no Arduino Mega
#define SS_PIN 10
#define RST_PIN 9

// Pinos do LED RGB e Buzzer
#define PIN_VERDE 2
#define PIN_VERMELHO 3
#define PIN_AZUL 11
#define BUZZER 4

MFRC522 rfid(SS_PIN, RST_PIN);

// UIDs cadastrados
const String UID_HOSPEDE   = "93 E6 27 03";
const String UID_CAMAREIRA = "67 2D 9A 5F";

// Memória de estado do pedido de serviço de quarto
bool pedidoAtivo = false;

// Função para converter o UID lido para String formatada
String obterUID(MFRC522 &mfrc522) {
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) uidString += " ";
  }
  uidString.toUpperCase();
  return uidString;
}

// Musiquinha de solicitação do hóspede (~3s)
void tocarMusicaSolicitado() {
  int melodia[] = { 1047, 1318, 1568, 2093, 1568, 2093 };
  int duracoes[] = { 300,  300,  300,  600,  300,  1200 };

  for (int i = 0; i < 6; i++) {
    tone(BUZZER, melodia[i]);
    delay(duracoes[i]);
    noTone(BUZZER);
    delay(50);
  }
}

// Som de cancelamento do hóspede (~0.7s)
void tocarSomCancelado() {
  tone(BUZZER, 400);
  delay(200);
  noTone(BUZZER);
  delay(100);
  
  tone(BUZZER, 300);
  delay(400);
  noTone(BUZZER);
}

// Bip ultra-rápido da camareira (100ms)
void tocarSomCamareira() {
  tone(BUZZER, 2500); // Tom agudo
  delay(100);          
  noTone(BUZZER);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_VERMELHO, OUTPUT);
  pinMode(PIN_AZUL, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Garante que tudo inicie desligado
  digitalWrite(PIN_VERDE, LOW);
  digitalWrite(PIN_VERMELHO, LOW);
  digitalWrite(PIN_AZUL, LOW);
  digitalWrite(BUZZER, LOW);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println(F("========================================"));
  Serial.println(F(" Sistema de Servico de Quarto Inicializado "));
  Serial.println(F("========================================"));
}

void loop() {
  // Aguarda aproximação de uma tag/cartão RFID
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  String uidLido = obterUID(rfid);
  // Pega apenas os 2 primeiros caracteres do UID (ex: "93" ou "67")
  String prefixoUID = uidLido.substring(0, 2);

  // --------------------------------------------------
  // 1. PIN AZUL (CAMAREIRA)
  // --------------------------------------------------
  if (uidLido == UID_CAMAREIRA) {
    if (pedidoAtivo) {
      // Finaliza o serviço pendente
      pedidoAtivo = false;

      digitalWrite(PIN_VERDE, LOW);
      digitalWrite(PIN_VERMELHO, LOW);
      digitalWrite(PIN_AZUL, HIGH);

      // Envia no formato "PREFIXO:STATUS" (Ex: 67:0)
      Serial.print(prefixoUID);
      Serial.println(F(":0"));

      // Som rápido no buzzer
      tocarSomCamareira();

      // Mantém o LED Azul por 3 segundos
      delay(2900); 
      digitalWrite(PIN_AZUL, LOW);

    } else {
      Serial.println(F("Nenhum pedido pendente para finalizar"));
    }

  // --------------------------------------------------
  // 2. CARTÃO BRANCO (HÓSPEDE)
  // --------------------------------------------------
  } else if (uidLido == UID_HOSPEDE) {
    if (!pedidoAtivo) {
      // 1º Toque: Solicitar Serviço
      pedidoAtivo = true;

      // Envia no formato "PREFIXO:STATUS" (Ex: 93:1)
      Serial.print(prefixoUID);
      Serial.println(F(":1"));

      digitalWrite(PIN_VERMELHO, LOW);
      digitalWrite(PIN_AZUL, LOW);
      digitalWrite(PIN_VERDE, HIGH);

      // Toca a música (~3s)
      tocarMusicaSolicitado();

      // Apaga o LED Verde
      digitalWrite(PIN_VERDE, LOW);

    } else {
      // 2º Toque: Cancelar Serviço
      pedidoAtivo = false;

      // Envia no formato "PREFIXO:STATUS" (Ex: 93:0)
      Serial.print(prefixoUID);
      Serial.println(F(":0"));

      digitalWrite(PIN_VERDE, LOW);
      digitalWrite(PIN_AZUL, LOW);
      digitalWrite(PIN_VERMELHO, HIGH);

      // Toca o som de cancelamento (~0.7s)
      tocarSomCancelado();

      // Completa o tempo total de 3s do LED Vermelho aceso
      delay(2300);
      digitalWrite(PIN_VERMELHO, LOW);
    }

  // --------------------------------------------------
  // 3. CARTÃO / TAG NÃO RECONHECIDO
  // --------------------------------------------------
  } else {
    Serial.print(F("Cartao nao cadastrado: "));
    Serial.println(uidLido);
  }

  // Finaliza a leitura e aguarda 1s antes do próximo ciclo
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(1000);
}