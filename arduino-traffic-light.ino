//#include <CurieTime.h>
/*
  
 Grupo: Israel e Oseias.
 
 Semaforo de transito com semaforo de pedestres.
 
 */

// numeros dos pinos
int c_r = 12;
int c_y = 11;
int c_g = 10;
int p_r = 9;
int p_g = 8;

// pino 0 da interrupçao eh o pino 2 digital
int int_button = 0;

// variavel de controle para aceitar somente uma diminuiçao no tempo do semaforo
int controller = 1;

// tempos da sinaleira

// vermelho para carros
long time_c_r = 10000;
// amarelo para carros
long time_c_y = 5000;
// verde para carros
long time_c_g = 15000;

// tempo que o botao ira diminuir do tempo da luz verde dos carros
long time_button = 10000;

// tempo que o sinal vermelho do pedestre pisca antes de ficar vermelho
long time_ped = 2500;

// variaveis de auxilio para calculo do tempo
unsigned long prev_time = 0, now_time = 0;

void setup() {                
  // inicializando os pinos
  pinMode(c_r, OUTPUT);
  pinMode(c_y, OUTPUT);
  pinMode(c_g, OUTPUT);
  pinMode(p_r, OUTPUT);
  pinMode(p_g, OUTPUT);
  pinMode(int_button+2, INPUT);

  // configuraçao da interrupçao pelo botao
  attachInterrupt(int_button, decrease_time, RISING);
}

void decrease_time(){
  if(digitalRead(c_g) == HIGH && controller == 1){
    prev_time -= time_button;
    controller = 0;
  }
}

void loop() {
  if(now_time == 0){
    // inicializa sinaleira
    digitalWrite(c_r, LOW);
    digitalWrite(c_g, HIGH);
    digitalWrite(c_y, LOW);
    digitalWrite(p_r, HIGH);
    digitalWrite(p_g, LOW);
  }else{
    if ((now_time - prev_time) >= time_c_g && digitalRead(c_g) == HIGH){
      // verde para amarelo (carros)
      digitalWrite(c_g, LOW);
      digitalWrite(c_y, HIGH);
      prev_time = now_time;
    }else if((now_time - prev_time) >= time_c_y && digitalRead(c_y) == HIGH){
      // amarelo para vermelho (carros)
      digitalWrite(c_y, LOW);
      digitalWrite(c_r, HIGH);
      delay(1000);
      // vermelho para verde (pedestres)
      digitalWrite(p_r, LOW);
      digitalWrite(p_g, HIGH);
      prev_time = now_time;
    }else if((now_time - prev_time) >= time_c_r && digitalRead(c_r) == HIGH){
      // vermelho (pedestres)
      digitalWrite(p_r, HIGH);
      delay(1000);
      // vermelho para verde(carros)
      digitalWrite(c_r, LOW);
      digitalWrite(c_g, HIGH);
      controller = 1;
      prev_time = now_time;
    }else if((now_time - prev_time) >= (time_c_r - time_ped) && digitalRead(c_r) == HIGH){
      // verde para vermelho (pedestres)
      digitalWrite(p_g, LOW);
      // pisca luz vermelha (pedestres)
      if ((now_time - prev_time) % 500 == 0){
        digitalWrite(p_r, !digitalRead(p_r));
      }
    }
  }
  now_time = millis();
}
