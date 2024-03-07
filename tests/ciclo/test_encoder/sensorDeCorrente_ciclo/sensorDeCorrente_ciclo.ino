//Sensor de Corrente ACS712 medindo Corrente DC, Potencia e Energia 
//Por GE Projetos e Tutoriais


float tensaoDC = 12.000; //Defina aqui a tensão DC que você está usando para alimentar seu circuito
float corrente; //A variável que vai armazenar a corrente medida pelo sensor
float potencia; //A variável que vai armazenar potência a partir da fórmula P = V x I
float energia=0; //A variável que vai armazenar a energia gasta a partir da fórmula E = P x t 

//Defina a variável 'sensibilidade' a partir da corrente medida pelo seu sensor:
//- Para 30A, sensibilidade = 0.066; 
//- Para 20A, sensibilidade = 0.100; 
//- Para 5A,  sensibilidade = 0.185;
float sensibilidade = 0.072; 

const int pino2 = 36;

void setup() {
  Serial.begin(115200);
  
}

void loop() {
  corrente = calculaCorrente(filtroDaMedia()); //Calcula a corrente
  potencia = abs(corrente * tensaoDC); //Calcula a potência a partir da fórmula P = V x I. A potência é apresentada em valor absoluto pela função "abs"
  energia+=(potencia*1.2/1000); //Calcula a energia gasta até o momento a partir da fórmula E = P x t  __  t = 1,2s ,pois esse é o tempo aproximado de ciclo da função loop desse programa
  //lcd.clear(); //Apaga todos os caracteres do display
  //lcd.setCursor(0,0); //Coloca o cursor do display na primeira linha e primeira coluna
  //Digita o valor de Tensão na linha 1
  //lcd.print("Tensao:    ");
  //lcd.print(tensaoDC,3);
  //lcd.print(" V");
  //Digita o valor de Corrente na linha 2
  //lcd.setCursor(0,1);    
  Serial.print("Corrente:   ");
  Serial.println(abs(corrente)-0.064,3);
  //lcd.print(" A");
 
}

// A função "calculaCorrente" vai converter o sinal amostrado pelo arduino em A0 num valor de corrente.
float calculaCorrente(int sinalSensor) {
  return (float)(sinalSensor-512)*(5.000)/(4096.000*sensibilidade); //508 por 512
}

// A função "filtroDaMedia", como o nome já diz, é um Filtro da Média. Esse é um filtro digital que serve para retirar o ruído do sinal do sensor.
// Nessa função, nós amostramos o sinal do sensor mil vezes, somamos essas amostras e dividimos por 1000. Obtemos assim a média aritmética de 1000 amostras.
int filtroDaMedia(){
  long somaDasCorrentes=0, mediaDasCorrentes;
  for(int i=0; i<1000; i++){
    somaDasCorrentes+=analogRead(pino2);
    delay(1);
  }
  mediaDasCorrentes=somaDasCorrentes/1000; 
  return mediaDasCorrentes;
}
