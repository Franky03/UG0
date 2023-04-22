/* Referência: 
 *  
// Plotclock
// cc - by Johannes Heberlein 2014
// v 1.02

*/

#include <TimeLib.h>  
#include <Servo.h>

//#define CALIBRATION     

#define WISHY 3 // Offset das coordenadas Y placa
//Quando estiver no modo calibração, ajustar até os servos moverem 90 graus
#define FaktorServoEsquerdo 600
#define FaktorServoDireito 600


//Ajustar os valores nulos para os braços estarem paralelos ao eixo x e y, quando estiver em modo de calibração
//Posições dos servos inicialmente
#define ServoEsquerdoNulo 1400//1650
#define ServoDireitoNulo 520 //540 //645

#define PinoServoLevanta  2
#define PinoServoEsquerdo  3
#define PinoServoDireito 4

#define ZOFF 150

#define LEVANTA0 1110+ZOFF // Na superficie
#define LEVANTA1 995+ZOFF  // Entre os números
#define LEVANTA2 735+ZOFF  // Pegar o apagador

// velocidade do braço de levantar a plataforma
#define VelocidadeLevanta 2000

//  Tamanho dos braços
#define L1 35
#define L2 55.1
#define L3 11.8
#define L4 35

//origem dos servos direito e esquerdo
#define O1X 24//22
#define O1Y -25
#define O2X 49//47
#define O2Y -25

int levanta_parametro = 1500;

Servo servo1;  // 
Servo servo2;  // 
Servo servo3;  // 

volatile double lastX = 55;
volatile double lastY = 37.5;

int ultimo_minuto = 0;

void setup() 
{ 
  
  pinMode(7, INPUT_PULLUP); 
  // Colocar a hora atual
  setTime(01,04,0,0,0,0);

  Posiciona(75.2, 47);
  Levanta(0);
    
  delay(1000);

} 

void loop() 
{ 

#ifdef CALIBRATION

   if (!servo1.attached()) servo1.attach(PinoServoLevanta);
    if (!servo2.attached()) servo2.attach(PinoServoEsquerdo);
    if (!servo3.attached()) servo3.attach(PinoServoDireito);

  //Os servos vão ter 90 graus entre os movementos, paralelos aos eixos x e y
  Posiciona(-3, 29.2);
  delay(500);
  Posiciona(74.1, 28);
  delay(500);

#else 
  int letter= 1;
  int i = 0;
  if (ultimo_minuto != minute()) {

    if (!servo1.attached()) servo1.attach(PinoServoLevanta);
    if (!servo2.attached()) servo2.attach(PinoServoEsquerdo);
    if (!servo3.attached()) servo3.attach(PinoServoDireito);

    //Levanta(0);

    hour();
    while ((i+1)*10 <= hour())
    {
      i++;
    }
    Levanta(1);

    if(letter){
      // UGO :|
        Letra(5, 25, 'U', 0.9);
        Letra(19, 25, 'G', 0.9);
        Letra(30, 25, 'O', 0.9);
        Numero(40, 25, 12, 0.9);
            
        //Letra(5, 25, '', 0.9);
        //Letra(19, 25, '', 0.9);
        //Letra(28, 25, '', 0.9);
        //Letra(40, 25, '', 0.9);
        //Numero(50, 25, 2, 0.9);
        
    }
    
    else{
      
      //Numero(3, 3, 111, 1);
      
      Numero(5, 25, i, 0.9);
      Numero(19, 25, (hour()-i*10), 0.9);
      Numero(28, 25, 11, 0.9);
    
      i=0;
      while ((i+1)*10 <= minute())
      {
        i++;
      }
      Numero(34, 25, i, 0.9);
      Numero(48, 25, (minute()-i*10), 0.9);
      
    }
    
    Levanta(2);
    Posiciona(71.0, 47.2);
    Levanta(1);
    ultimo_minuto = minute();
    delay(580);
    servo1.detach();
    servo2.detach();
    servo3.detach();
    
  }

#endif

} 

// Letras baseadas no plano cartesiano

void Letra(float bx, float by, char letra, float scale){
  switch(letra) {
   
    case 65: // letra A
    case 97: 
      Posiciona(bx + 0 * scale, by + 0 * scale);
      delay(200);
      Levanta(0);
      delay(200);
      Posiciona(bx + 0 * scale, by + 20 * scale);
      delay(200);
      Posiciona(bx + 10.3 * scale, by + 6 * scale);
      delay(200);
      Posiciona(bx + 0.7 * scale, by + 6 * scale);
      delay(200);
      Posiciona(bx + 8.4 * scale, by + 6 * scale);
      delay(200);
      Posiciona(bx + 11 * scale, by - 1 * scale);
      delay(200);
      Levanta(1);
      break;
      
    case 66: // B
    case 98:
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Levanta(0);
      Posiciona(bx + 0 * scale, by + 20 * scale);
      SemiCirculo(bx + 5 * scale, by + 15 * scale, 5 * scale, 3, -2, 1);
      SemiCirculo(bx + 5 * scale, by + 5 * scale, 5 * scale, 1.57, -3, 1);
      Levanta(1);
      break;
      
    case 67: //C
    case 99:
      Posiciona(bx + 7 * scale, by + 20 * scale);
      Levanta(0);
      Posiciona(bx + 0 * scale, by + 20 * scale);
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Posiciona(bx + 7 * scale, by + 0 * scale);
      Levanta(1);
      break;

    case 68: //D
    case 100:
      Posiciona(bx + 0 * scale, by +0 * scale);
      Levanta(0);
      
      Posiciona(bx + 0 * scale, by +20 * scale);
      delay(200);
      Posiciona(bx + 6 * scale, by +16 * scale);
      Posiciona(bx + 10 * scale, by +12 * scale);
      Posiciona(bx + 6 * scale, by + 8 * scale);
      Posiciona(bx + 0 * scale, by + 0 * scale);
      
      //SemiCirculo(bx + 6 * scale, by + 3 * scale, 8 * scale, 3, -3, 0.8);
      //Posiciona(bx -1 * scale, by + 10 * scale);
      Levanta(1);
      break;
      
    case 69: //E
    case 101:
        Posiciona(bx + 8 * scale, by + 20 * scale);
        Levanta(0);
        Posiciona(bx + 0 * scale, by + 20 * scale);
        Posiciona(bx + 0 * scale, by + 10 * scale);
        Posiciona(bx + 8 * scale, by + 10 * scale);
        Posiciona(bx + 0 * scale, by + 10 * scale);
        Posiciona(bx + 0 * scale, by + 0 * scale);
        Posiciona(bx + 8 * scale, by + 0 * scale);
        Levanta(1);
        break;
        
     case 70: // F
     case 102:
        Posiciona(bx + 0 * scale, by + 0 * scale);
        Levanta(0);
        Posiciona(bx + 0 * scale, by + 10 * scale);
        Posiciona(bx + 6 * scale, by + 10 * scale);
        Posiciona(bx + 0 * scale, by + 10 * scale);
        Posiciona(bx + 0 * scale, by + 20 * scale);
        Posiciona(bx + 6 * scale, by + 20 * scale);
        Levanta(1);
        break;
        
    case 71: //G
    case 103:
      Posiciona(bx + 8 * scale, by + 20 * scale);
      Levanta(0);
      Posiciona(bx + 0 * scale, by + 20 * scale);
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Posiciona(bx + 8 * scale, by + 0 * scale);
      Posiciona(bx + 8 * scale, by + 10 * scale);
      Posiciona(bx + 4 * scale, by + 10 * scale);
      Posiciona(bx + 10 * scale, by + 10 * scale);
      Levanta(1);
      break;
    case 72: //H
    case 104:
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Levanta(0);
      Posiciona(bx + 0 * scale, by + 20 * scale);
      Posiciona(bx + 0 * scale, by + 10 * scale);
      Posiciona(bx + 8 * scale, by + 10 * scale);
      Posiciona(bx + 8 * scale, by + 20 * scale);
      Posiciona(bx + 8 * scale, by + 0 * scale);
      Levanta(1);
      break;
      
    case 73: // I
    case 105:
      Posiciona(bx + 3 * scale, by + 0 * scale);
      Levanta(0);
      Posiciona(bx + 3 * scale, by + 20 * scale);
      //Circulo(bx + 4 * scale, by + 20 * scale, 0.1 * scale, 1, -1, 1);
      Levanta(1);
      break;

    case 74: // J
    case 106:
      Posiciona(bx + 0 * scale, by + 10 * scale);
      Levanta(0);
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Posiciona(bx + 8 * scale, by + 0 * scale);
      Posiciona(bx + 8 * scale, by + 20 * scale);
      Levanta(1);
      break;

    case 75: // K
    case 107:
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Levanta(0);
      Posiciona(bx + 0 * scale, by + 20 * scale);
      delay(200);
      Posiciona(bx + 0 * scale, by + 10 * scale);
      delay(200);
      Posiciona(bx + 6 * scale, by + 20 * scale);
      Levanta(1);
      delay(200);
      Posiciona(bx + 0 * scale, by + 10 * scale);
      Levanta(0);
      delay(200);
      Posiciona(bx + 6 * scale, by + 0 * scale);
      Levanta(1);
      break;
      
    case 76: //L
    case 108:
      Posiciona(bx + 0 * scale, by + 20 * scale);
      Levanta(0);
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Posiciona(bx + 8 * scale, by + 0 * scale);
      Levanta(1);
      break;
    case 77: //M
    case 109:
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Levanta(0);
      Posiciona(bx + 0 * scale, by + 20 * scale);
      delay(200);
      Posiciona(bx + 4 * scale, by + 10 * scale);
      delay(200);
      Posiciona(bx + 8 * scale, by + 20 * scale);
      Posiciona(bx + 8 * scale, by + 0 * scale);
      Levanta(1);
      break;
      
    case 78: //Letra N
    case 110:
      Posiciona(bx + 0 * scale, by + 0 * scale);
      delay(200);
      Levanta(0);
      Posiciona(bx + 0 * scale, by + 20 * scale);
      delay(200);
      Posiciona(bx + 6 * scale, by + 0 * scale);
      delay(200);
      Posiciona(bx + 6 * scale, by + 20 * scale);
      delay(200);
      Levanta(1);
      break;
      
    case 79: // O
    case 111:
      Posiciona(bx + 12 * scale, by + 6 * scale);
      Levanta(0);
      Circulo(bx + 7 * scale, by + 10 * scale, 10 * scale, -0.8, 6.7, 0.5);
      Levanta(1);
      break;
    
    case 80://Letra P
    case 112:
      Posiciona(bx + 6 * scale, by +0 * scale);
      Levanta(0);
      SemiCirculo(bx + 8 * scale, by + 10 * scale, 5 * scale, 3, -2, 1);
      Levanta(1);
      break;
    case 81: //Q
    case 113:
      Posiciona(bx + 12 * scale, by + 6 * scale);
      Levanta(0);
      Circulo(bx + 7 * scale, by + 10 * scale, 10 * scale, -0.8, 6.7, 0.5);
      Posiciona(bx + 14 * scale, by - 2  * scale);
      Levanta(1);
      break;
    case 82: //R
    case 114:
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Levanta(0);
      Posiciona(bx + 0 * scale, by + 20 * scale);
      SemiCirculo(bx + 5 * scale, by + 15 * scale, 5 * scale, 3, -2, 1);
      Posiciona(bx + 14  * scale, by + 0 * scale);
      Levanta(1);
      break;
      
    case 83: //S
    case 115:
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Levanta(0);
      Posiciona(bx + 8 * scale, by + 0 * scale);
      Posiciona(bx + 8 * scale, by + 10 * scale);
      Posiciona(bx + 0 * scale, by + 10 * scale);
      Posiciona(bx + 0 * scale, by + 20 * scale);
      Posiciona(bx + 8 * scale, by + 20 * scale);
      Levanta(1);
      break;
      
    case 84: // lETRA T
    case 116: 
      Posiciona(bx + 0 * scale, by + 0 * scale);
      Levanta(0);
      
      Posiciona(bx + 0 * scale, by + 20 * scale);
      Posiciona(bx - 8 * scale, by + 20 * scale);
      Posiciona(bx + 8 * scale, by + 20 * scale);
      Levanta(1);
      break;
      
    case 85: //U
    case 117:
       Posiciona(bx + 0 * scale, by + 20 * scale);
       Levanta(0);
       Posiciona(bx + 0 * scale, by + 0 * scale);
       Posiciona(bx + 8 * scale, by + 0 * scale);
       Posiciona(bx + 8 * scale, by + 20 * scale);
       Levanta(1);

       break;
       
     case 86: // V
     case 118:
        Posiciona(bx + 0 * scale, by + 20 * scale);
        Levanta(0);
        Posiciona(bx + 4 * scale, by + 0 * scale);
        Posiciona(bx + 8 * scale, by + 20 * scale);
        Levanta(1);
        break;
     case 87: //W
     case 119:
        Posiciona(bx + 0 * scale, by + 20 * scale);
        Levanta(0);
        Posiciona(bx + 3 * scale, by + 0 * scale);
        delay(100);
        Posiciona(bx + 5 * scale, by + 10 * scale);
        delay(100);
        Posiciona(bx + 8 * scale, by + 0 * scale);
        delay(100);
        Posiciona(bx + 8 * scale, by + 20 * scale);
        Levanta(1);
        break;

     case 88: // X
     case 120:
        Posiciona(bx + 0 * scale, by + 0 * scale);
        Levanta(0);
        Posiciona(bx + 8 * scale, by + 20 * scale);
        Levanta(1);
        Posiciona(bx + 0 * scale, by + 20 * scale);
        Levanta(0);
        Posiciona(bx + 8 * scale, by + 0 * scale);
        Levanta(1);
        break;
        
     case 89: //Y
     case 121:
        Posiciona(bx + 0 * scale, by + 20 * scale);
        Levanta(0);
        Posiciona(bx + 4 * scale, by + 10 * scale);
        Posiciona(bx + 10 * scale, by + 20 * scale);
        delay(200);
        Posiciona(bx + 0 * scale, by + 0 * scale);
        Levanta(1);
        break;
        
     case 90://Z
     case 122: 
      Posiciona(bx + 12 * scale, by +0 * scale);
      delay(200);
      Levanta(0);
      delay(200);
      Posiciona(bx + 0 * scale, by +0 * scale);
      delay(200);
      Posiciona(bx + 12 * scale, by +20 * scale);
      delay(200);
      Posiciona(bx + 0 * scale, by +20 * scale);
      delay(200);
      Levanta(1);
      break;
  
  }

}

// Escrevendo numeral com bx by sendo o ponto de origem esquerdo
// Escala 1 é igual uma fonte de 20 mm
// A estrutura segue o caminho: move para o primeiro ponto inicial do numeral, abaixa o lápis desenha o numero e levanta o lápis

void Numero(float bx, float by, int num, float scale) {
  // O padrão é: ele faz o movimento inicial para o começo do desenho
  // Depois ele abaixa o lápis e começa a mover pela mesa
  // Depois ele levanta o lápis de novo
  
  switch (num) {
  case 0:
    Posiciona(bx + 12 * scale, by + 6 * scale);
    Levanta(0);
    Circulo(bx + 7 * scale, by + 10 * scale, 10 * scale, -0.8, 6.7, 0.5);
    Levanta(1);
    break;
  case 1:

    Posiciona(bx + 3 * scale, by + 15 * scale);
    Levanta(0);
    Posiciona(bx + 10 * scale, by + 20 * scale);
    Posiciona(bx + 10 * scale, by + 0 * scale);
    Levanta(1);
    break;
  case 2:
    Posiciona(bx + 2 * scale, by + 12 * scale);
    Levanta(0);
    SemiCirculo(bx + 8 * scale, by + 14 * scale, 6 * scale, 3, -0.8, 1);
    Posiciona(bx + 1 * scale, by + 0 * scale);
    Posiciona(bx + 12 * scale, by + 0 * scale);
    Levanta(1);
    break;
  case 3:
    Posiciona(bx + 2 * scale, by + 17 * scale);
    Levanta(0);
    SemiCirculo(bx + 5 * scale, by + 15 * scale, 5 * scale, 3, -2, 1);
    SemiCirculo(bx + 5 * scale, by + 5 * scale, 5 * scale, 1.57, -3, 1);
    Levanta(1);
    break;
  case 4:
    Posiciona(bx + 10 * scale, by + 0 * scale);
    Levanta(0);
    Posiciona(bx + 10 * scale, by + 20 * scale);
    Posiciona(bx + 2 * scale, by + 6 * scale);
    Posiciona(bx + 12 * scale, by + 6 * scale);
    Levanta(1);
    break;
  case 5:
    Posiciona(bx + 2 * scale, by + 5 * scale);
    Levanta(0);
    Circulo(bx + 5 * scale, by + 6 * scale, 6 * scale, -2.5, 2, 1);
    Posiciona(bx + 5 * scale, by + 20 * scale);
    Posiciona(bx + 12 * scale, by + 20 * scale);
    Levanta(1);
    break;
  case 6:
    Posiciona(bx + 2 * scale, by + 10 * scale);
    Levanta(0);
    SemiCirculo(bx + 7 * scale, by + 6 * scale, 6 * scale, 2, -4.4, 1);
    Posiciona(bx + 11 * scale, by + 20 * scale);
    Levanta(1);
    break;
  case 7:
    Posiciona(bx + 2 * scale, by + 20 * scale);
    Levanta(0);
    Posiciona(bx + 12 * scale, by + 20 * scale);
    Posiciona(bx + 2 * scale, by + 0);
    Levanta(1);
    break;
  case 8:
    Posiciona(bx + 5 * scale, by + 10 * scale);
    Levanta(0);
    SemiCirculo(bx + 5 * scale, by + 15 * scale, 5 * scale, 4.7, -1.6, 1);
    Circulo(bx + 5 * scale, by + 5 * scale, 5 * scale, -4.7, 2, 1);
    Levanta(1);
    break;

  case 9:
    Posiciona(bx + 9 * scale, by + 11 * scale);
    Levanta(0);
    SemiCirculo(bx + 7 * scale, by + 15 * scale, 5 * scale, 4, -0.5, 1);
    Posiciona(bx + 5 * scale, by + 0);
    Levanta(1);
    break;
    
  // 888 é o comando para apagar a lousa
  case 888:

    Levanta(0);
    Posiciona(70, 45);
    Posiciona(65-WISHY, 43);

    Posiciona(65-WISHY, 46);
    Posiciona(5, 49);
    Posiciona(5, 46);
    Posiciona(63-WISHY, 46);
    Posiciona(63-WISHY, 42);

    Posiciona(5, 42);
    Posiciona(5, 38);
    Posiciona(63-WISHY, 38);
    Posiciona(63-WISHY, 34);

    Posiciona(5, 34);
    Posiciona(5, 29);
    Posiciona(6, 29);
    Posiciona(65-WISHY, 26);

    Posiciona(5, 26);
    Posiciona(60-WISHY, 40);

    Posiciona(73.2, 44.0);
    Levanta(2);

    break;
    
  // Número 11 é o comando pra gente desenhar os dois pontos
  
  case 11:
    Posiciona(bx + 5 * scale, by + 15 * scale);
    Levanta(0);
    Circulo(bx + 5 * scale, by + 15 * scale, 0.1 * scale, 1, -1, 1);
    delay(10);
    Levanta(1);
    Posiciona(bx + 5 * scale, by + 5 * scale);
    Levanta(0);
    Circulo(bx + 5 * scale, by + 5 * scale, 0.1 * scale, 1, -1, 1);
    delay(10);
    Levanta(1);
    break;

   case 12: // Carinha do UGO
    Posiciona(bx + 5 * scale, by + 15 * scale);
    Levanta(0);
    Circulo(bx + 5 * scale, by + 15 * scale, 0.1 * scale, 1, -1, 1);
    delay(10);
    Levanta(1);
    Posiciona(bx + 10 * scale, by + 15 * scale);
    Levanta(0);
    Circulo(bx + 10 * scale, by + 15 * scale, 0.1 * scale, 1, -1, 1);
    delay(10);
    Levanta(1);

    Posiciona(bx + 5 * scale, by + 8 * scale);
    Levanta(0);
    Posiciona(bx + 10 * scale, by + 5 * scale);
    Posiciona(bx + 15 * scale, by + 8 * scale);
    Levanta(1);
    break;

  }
  
}


void Levanta(char Levanta) {
  switch (Levanta) {

  case 0: //850

      if (levanta_parametro >= LEVANTA0) {
      while (levanta_parametro >= LEVANTA0) 
      {
        levanta_parametro--;
        servo1.writeMicroseconds(levanta_parametro);				
        delayMicroseconds(VelocidadeLevanta);
      }
    } 
    else {
      while (levanta_parametro <= LEVANTA0) {
        levanta_parametro++;
        servo1.writeMicroseconds(levanta_parametro);
        delayMicroseconds(VelocidadeLevanta);

      }

    }

    break;

  case 1: //150

    if (levanta_parametro >= LEVANTA1) {
      while (levanta_parametro >= LEVANTA1) {
        levanta_parametro--;
        servo1.writeMicroseconds(levanta_parametro);
        delayMicroseconds(VelocidadeLevanta);

      }
    } 
    else {
      while (levanta_parametro <= LEVANTA1) {
        levanta_parametro++;
        servo1.writeMicroseconds(levanta_parametro);
        delayMicroseconds(VelocidadeLevanta);
      }

    }

    break;

  case 2:

    if (levanta_parametro >= LEVANTA2) {
      while (levanta_parametro >= LEVANTA2) {
        levanta_parametro--;
        servo1.writeMicroseconds(levanta_parametro);
        delayMicroseconds(VelocidadeLevanta);
      }
    } 
    else {
      while (levanta_parametro <= LEVANTA2) {
        levanta_parametro++;
        servo1.writeMicroseconds(levanta_parametro);				
        delayMicroseconds(VelocidadeLevanta);
      }
    }
    break;
  }
}


void SemiCirculo(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = -0.05;
  float count = 0;

  do {
    Posiciona(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) > ende);

}

void Circulo(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = 0.05;
  float count = 0;

  do {
    Posiciona(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) <= ende);
}


void Posiciona(double pX, double pY) {
  double dx, dy, c;
  int i;

  // dx dy do novo ponto
  dx = pX - lastX;
  dy = pY - lastY;
  
  c = floor(7 * sqrt(dx * dx + dy * dy));

  if (c < 1) c = 1;

  for (i = 0; i <= c; i++) {
    // desenha a linha ponto por ponto
    define_XY(lastX + (i * dx / c), lastY + (i * dy / c));

  }

  lastX = pX;
  lastY = pY;
}

double retorna_angulo(double a, double b, double c) {
   // Lei dos cosenos para angulo ente c e a
  return acos((a * a + c * c - b * b) / (2 * a * c));
}

void define_XY(double Tx, double Ty) 
{
  delay(1);
  double dx, dy, c, a1, a2, Hx, Hy;

  // Calcular triangulo entre lápis, servo esquerdo e articulação do braço
  
  // dx/dy cartesiano
  dx = Tx - O1X;
  dy = Ty - O1Y;

  // angulo a1 e comprimento c
  c = sqrt(dx * dx + dy * dy); // 
  a1 = atan2(dy, dx); //
  a2 = retorna_angulo(L1, L2, c);

  servo2.writeMicroseconds(floor(((a2 + a1 - M_PI) * FaktorServoEsquerdo) + ServoEsquerdoNulo));

// Calcular ponto da articulação do braço para o triangulo do braço direito do servo
  a2 = retorna_angulo(L2, L1, c);
  Hx = Tx + L3 * cos((a1 - a2 + 0.621) + M_PI); //36,5°
  Hy = Ty + L3 * sin((a1 - a2 + 0.621) + M_PI);

//Calcular triangulo entre articulação do lápis, servo direito e articulação do braço
  dx = Hx - O2X;
  dy = Hy - O2Y;

  c = sqrt(dx * dx + dy * dy);
  a1 = atan2(dy, dx);
  a2 = retorna_angulo(L1, L4, c);

  servo3.writeMicroseconds(floor(((a1 - a2) * FaktorServoDireito) + ServoDireitoNulo));

}
