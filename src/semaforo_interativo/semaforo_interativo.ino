/**
 * *************************************************************************
 * @ Software: Semaforo interativo
 * @ Date: 21/09/2016
 * @ Version: 0.0.2
 * @ Licese; MIT
 * @ Autor: Prof. Alexsandre
 * @
 * ************************************************************************/


#include <Servo.h> //Incluindo a biblioteca servo motor

//Cria objetos servos
Servo servo1, servo2;

// Configura byte digito.
byte seven_seg_digits[10][7] = {
  { 0, 0, 0, 0, 0, 0, 1 }, // = 0
  { 1, 0, 0, 1, 1, 1, 1 }, // = 1
  { 0, 0, 1, 0, 0, 1, 0 }, // = 2
  { 0, 0, 0, 0, 1, 1, 0 }, // = 3
  { 1, 0, 0, 1, 1, 0, 0 }, // = 4
  { 0, 1, 0, 0, 1, 0, 0 }, // = 5
  { 0, 1, 0, 0, 0, 0, 0 }, // = 6
  { 0, 0, 0, 1, 1, 1, 1 }, // = 7
  { 0, 0, 0, 0, 0, 0, 0 }, // = 8
  { 0, 0, 0, 1, 1, 0, 0 }  // = 9
};

//Declarações de variaveis
#define SIRENE         44  //Declarando o pino 3 como inteiro e nomeando como SIRENE(pino a ode vai ser ligado o buzzer)
#define CAR_RED        18  //Definindo o pino 12 como inteiro e nomeando como Red(led vermelho para carros)
#define CAR_YELLOW     19  //Definindo o pino 11 como inteiro e nomeando como Yellow(led amarelo para carros)
#define CAR_GREEN      20  //Definindo o pino 10 como inteiro e nomeando como Green(led verde para carros)
#define PED_RED        35  //Definindo o pino 9 como inteiro e nomeando como Red(led vermelho para pedestre)
#define PED_GREEN      37  //Definindo o pino 8 como inteiro e nomeando como Green(led verde para pedestre)
#define BUTTON         43  //Definindo o pino 2 como inteiro e nomeando como BUTTON(botão que libera o sinal para o pedestre)
#define crossTime   10000  //tempo de travessia
#define TIME_DELAY   4000  //tempo de espera para feixar o semafaro dos veículos e abrir o do pedestre. 

unsigned long changeTime;// tempo desde a ultima vez que o botao foi pressionado
byte x, y;


//Função de configuração dos pinos
void setup()
{
  Serial.begin(9600);

  servo1.attach(21); // pin servo1 arduino
  servo2.attach(22); // pin servo2 arduino

  servo1.write(0); //posição inicial do servo1
  servo2.write(180); //posição inicial do servo2

  pinMode(SIRENE, OUTPUT);
  pinMode(CAR_RED, OUTPUT);
  pinMode(CAR_YELLOW, OUTPUT);
  pinMode(CAR_GREEN, OUTPUT);
  pinMode(PED_RED, OUTPUT);
  pinMode(PED_GREEN, OUTPUT);
  pinMode(BUTTON, INPUT);

  digitalWrite(CAR_GREEN, HIGH);
  digitalWrite(PED_RED, HIGH);

  //Configura Pinos do Display como saida, do pino 2 ao 17.

  for(int x = 2; x <= 17; x++){
    pinMode(x, OUTPUT);
  }

  writeDot1(0);  // Inicia com o "ponto" (o signal de casas decimais led) desligado
  writeDot2(0);

  /* configura cancela inicialmente aberta. */
  downServo1();
  downServo2();
}

/**
 * loop - 
 */
void loop()
{
  int state = digitalRead(BUTTON);

  if (state == HIGH && (millis() - changeTime) > 5000) {/* verifica se o botão foi pressionado e se faz mais de 5 seg desde a ultima vez. */
    delay(TIME_DELAY);    //aguarda 4 segundos
    changeLights(); //chama a funcao que altera as luzes
  }

}

/**
 * changeLights
 */
void changeLights() {

  digitalWrite(CAR_GREEN, LOW); //apaga CAR_GREEN
  digitalWrite(CAR_YELLOW, HIGH); //acende CAR_YELLOW
  delay(TIME_DELAY);

  digitalWrite(CAR_YELLOW, LOW); //apaga CAR_YELLOW
  digitalWrite(CAR_RED, HIGH); //acende CAR_RED
  
  //levanta cancela
  upServo1();
  upServo2();

  digitalWrite(SIRENE, HIGH);
  delay(300);
  digitalWrite(SIRENE, LOW);
  digitalWrite(PED_RED, LOW); //apaga PED_RED
  digitalWrite(PED_GREEN, HIGH); //acende PED_GREEN

  // Conta tempo.
  for (x = 3; x > 0; --x) {
    sevenSegWrite1(x - 1);
    for ( y = 10; y > 0; --y) {

      if((x <= 1) && (y <= 9)){//verifica se o tempo está acabando.
        alertaIntermitente(); // chama alerta interminte sonoro.
      }

      if((x <= 1) && (y <= 0)){//verifica se tempo acabou e baixa cancela.
        upServo1();
        upServo2();
      }

      sevenSegWrite2(y - 1);
      delay(1000);
    }
  }

  digitalWrite(PED_RED, HIGH); //acende PED_RED
  digitalWrite(CAR_RED, LOW); //apaga CAR_RED
  digitalWrite(CAR_GREEN, HIGH); //acende CAR_GREEN
  
  //baixa cancela.
  downServo1();
  downServo2();

  changeTime = millis();
}

void upServo1() {
  servo1.write(0);
}

void downServo1() {
  servo1.write(90);
}

void upServo2() {
  servo2.write(180);
}

void downServo2() {
  servo2.write(90);
}

void writeDot1(byte dot) {
  digitalWrite(9, dot);
}

void writeDot2(byte dot) {
  digitalWrite(17, dot);
}

void sevenSegWrite1(byte digit) {
  byte pin = 2;
  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(pin, seven_seg_digits[digit][segCount]);
    ++pin;
  }
}

void sevenSegWrite2(byte digit) {
  byte pin = 10;
  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(pin, seven_seg_digits[digit][segCount]);
    ++pin;
  }
}

void alertaIntermitente(){
  digitalWrite(PED_GREEN, HIGH);
  digitalWrite(SIRENE, HIGH);
  delay(100);

  digitalWrite(PED_GREEN, LOW);
  digitalWrite(SIRENE, LOW);
}

