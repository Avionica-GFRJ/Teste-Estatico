// Software para bancada de teste estático
// Autor: Gil Pinheiro - 05/10/2019
// Modificações: Renan Larrieu - 10/02/2020
// Modificações: Gabriel Maximus - 10/11/2022 - Adição da função de temperatura com termopar
// Realiza leituras de empuxo e pressão na câmara
// a cada 50 ms
// frequência de captação inferior a 10Hz
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Q2HX711.h>
//------------------------------------------------------------
const float calibrador = -1000;//Mudar aqui o número de calibração da célula de carga no 'calibrador'
//------------------------------------------------------------
const int  Tscan = 100; // Scan time in milisseconds
//const int  pressIn = A0;  // Pressure input
const byte hx711_data_pin = A1; 
const byte hx711_clock_pin = A0;
//const byte hx711_data_pin = 2; //esses dois pinos, data e clock são programáveis para utilização do sensoriamento de pressão futuramente
//const byte hx711_clock_pin = 3;
const float span = 10; //constante relacionada com as propriedades mecânicas da célula de carga
const float k1 = (500/4)*5.0/1023.0; //usado para captação de pressão
const float k2 = (500/4)*0.5;  //usado para captação de pressão
long  cont=0;
float emp0=0,pre0=0;
float empuxo, pressure;

//--------------------------------------------------------------------
//Termopar
#include "max6675.h" //INCLUSÃO DE BIBLIOTECA
 
int ktcSO = 8; //PINO DIGITAL (SO)
int ktcCS = 9; //PINO DIGITAL (CS)
int ktcCLK = 10; //PINO DIGITAL (CLK / SCK)
 
MAX6675 ktc(ktcCLK, ktcCS, ktcSO); //CRIA UMA INSTÂNCIA UTILIZANDO OS PINOS (CLK, CS, SO)

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);

void setup() {
  Serial.begin(115200);
  int i;
  for(i=0; i<10;i++){
    emp0 += -hx711.read();
   // pre0 += k1*analogRead(pressIn)-k2;
    delay(100); //delay define a frequência de aquisição. se delay(x),x=100 então a frequência de aquisição foi definida para 10Hz, pois o período é de 100ms. 
    //Para que a frequência de aquisição seja trocada para 80Hz nesse código, basta mudar o delay(x), isto é, x= 12.5ms
  }  
  
  emp0 = emp0/i; //media das medidas de empuxo
//  pre0 = pre0/i; //media das medidas de pressao
}

void loop() {
  
  long miliseg = millis();
  float seg = (float)miliseg/1000.0;
  
  // Pressure sensor range 0 to 500 psig
  //pressure = 500*(5/4)*(5*analogRead(pressIn)/1023.0-0.5);
//  pressure = k1*analogRead(pressIn)-k2-pre0;
  //pressure = (500.0/4.0)*(5.0*analogRead(pressIn)/1023.0-0.5);
  empuxo = span*2*((-hx711.read()-emp0)/(128*(-calibrador)));//Aqui fica o parâmetro de calibração para obter os dados de kgf
  //Serial.print(cont++);
  //Serial.print(" ;");
  Serial.print("Tempo: ");
  Serial.print(seg,3); //printa o tempo, com 3 casas decimais
  Serial.println(";");
  Serial.print("Empuxo: ");
  Serial.print(empuxo,5); //(printa os dados, número de casas decimais), no monitor serial!
 //Serial.print("kg");
 // Serial.print(" ;");
//  Serial.print(pressure);
//Serial.print("psi");
  Serial.println();
  Serial.print("Temperatura: "); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(ktc.readCelsius()); //IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA
  Serial.println("*C"); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.println("--------------------------------------------------------");
  delay(500); //INTERVALO DE 500 MILISSEGUNDOS
}
  
  
  /*if(dataFile){
    
      dataFile.print(seg,3); //printa o tempo, com 3 casas decimais
      //dataFile.print(seg);
      dataFile.print(";");
      dataFile.print(empuxo,5); //(printa os dados, número de casas decimais)
      dataFile.print('\n'); //pula linha
      dataFile.close();  
      }
      
      delay(Tscan); 
      
      } 
*/
