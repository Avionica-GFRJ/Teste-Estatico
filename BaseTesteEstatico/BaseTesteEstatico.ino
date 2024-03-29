#include "HX711.h" //INCLUSÃO DE BIBLIOTECA CELULA DE CARGA
#include "max6675.h" //INCLUSÃO DE BIBLIOTECA TERMOPAR

// HX711 Conexao
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

// MAX6675 Conexao
int ktcSO = 8; //PINO DIGITAL (SO)
int ktcCS = 9; //PINO DIGITAL (CS)
int ktcCLK = 10; //PINO DIGITAL (CLK / SCK)

HX711 scale; 
MAX6675 ktc(ktcCLK, ktcCS, ktcSO); //CRIA UMA INSTÂNCIA UTILIZANDO OS PINOS (CLK, CS, SO)

int cont = 9;
float t = 0;

void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {

  //   long tempo = millis();
  Serial.print(millis());
  Serial.print("\t");

  if (scale.is_ready()) {
    double reading = ((scale.read() - 289300.0)*3.0/(306700.0 - 289300.0)+0.87);
    Serial.print("HX711 reading: ");
    Serial.print(reading);
    Serial.print(" Kg");
    Serial.print("\t");
  } else {
    Serial.print("HX711 not found.");
  }  
   if(cont++==9){
   t = ktc.readCelsius();
   cont = 0; 
   }
   Serial.print("Temperatura: "); //IMPRIME O TEXTO NO MONITOR SERIAL
   Serial.print(t); //IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA
   Serial.print(" °C"); //IMPRIME O TEXTO NO MONITOR SERIAL
   Serial.print("\t");
   
   double sensorPValue = (1000/4.0)*((5.0*analogRead(A0)/1023.0)-0.55)+15.31; //Valor em PSI
   //double sensorPValue = (5.0*analogRead(A0)/1023.0); //Valor em Volts
   Serial.print("Pressão: "); //IMPRIME O TEXTO NO MONITOR SERIAL
   Serial.print(sensorPValue);
   Serial.println(" psi"); //IMPRIME O TEXTO NO MONITOR SERIAL
   
   
   
   //Serial.println(millis()-tempo);
  delay(100);
  
}
