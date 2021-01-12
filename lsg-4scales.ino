//                          Simon De Jaeger 20/08/2020
//                          Maxime Franco   12/01/2021
//                          ---------------------------    
//                        PROGRAMME ARDUINO POUR Balance à 4 pesons aux coins d'un rectangle.
//
//        balance 1=   clk:D2  dat:D3  lm35:A1   coefcalib=-440    coefth=   à 22.45 degrés
//        balance 2=   clk:D4  dat:D5  lm35:A2   coefcalib=-425    coefth=   à 22.45 degrés
//        balance 3=   clk:D6  dat:D7  lm35:A3   coefcalib=-425    coefth=   à 22.45 degrés
//        balance 4=   clk:D8  dat:D9  lm35:A3   coefcalib=-425    coefth=   à 22.45 degrés
//
//        Correction thermique optionelle dans ce programme
//
//
//        origine du repère B4, puis dans le sens horloger, B1,B2,B3.
//
//                           
//                       ----------------------------------

#include "HX711.h"
#include "math.h"

HX711 scales[4];
float calibration_factors[] = {440, 425, 425, 425};
const int LM35_PINS[] = {A1, A2, A3, A4};
const int SCALES_ID[][2] = {{3,2},{7,6},{5,4},{9,8}};
const int NB_SCALES = 4;
long zero_factors[]= {0,0,0,0};
float previous=0, actual=0;
const long LOADCELL_DIVIDER = 5895655;
const long LOADCELL_OFFSET = 50682624;
const int CLK = 1000;

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < NB_SCALES; i++){
    scales[i].begin(SCALES_ID[i][0], SCALES_ID[i][1]);
    scales[i].set_scale(calibration_factors[i]);
    //scales[i].set_offset(LOADCELL_OFFSET);
    scales[i].tare();
    zero_factors[i] = scales[i].read_average();
  } 
}

void loop() {
  if(Serial.available()) {
    char temp = Serial.read();
    switch(temp){
      case 'a':
        calibration_factors[0] += 10;
        break;
      case 'z':
        calibration_factors[0] += 1000;
        break;
      case 'e':
        calibration_factors[0] -= 1000;
        break;
      case 'r':
        calibration_factors[0] -= 10;
        break;
      case 'q':
        calibration_factors[1] += 10;
        break;
      case 's':
        calibration_factors[1] += 1000;
        break;
      case 'd':
        calibration_factors[1] -= 1000;
        break;
      case 'f':
        calibration_factors[1] -= 10;
        break;
      case 'w':
        calibration_factors[2] += 10;
        break;
      case 'x':
        calibration_factors[2] += 1000;
        break;
      case 'c':
        calibration_factors[2] -= 1000;
        break;
      case 'v':
        calibration_factors[2] -= 10;
        break;
      case 't':
        calibration_factors[3] += 10;
        break;
      case 'y':
        calibration_factors[3] += 1000;
        break;
      case 'u':
        calibration_factors[3] -= 1000;
        break;
      case 'i':
        calibration_factors[3] -= 10;
        break;
      case '.':
        Serial.print("Doing a tare... ");
        for(int i=0; i<NB_SCALES; i++){
          scales[i].tare();
        }
        delay(CLK);
        Serial.println("Done");
        break;
    }
    for(int i = 0; i < NB_SCALES; i++){
      scales[i].set_scale(calibration_factors[i]);
      scales[i].tare();
      zero_factors[i] = scales[i].read_average();
    }     
  }
  int temp_adc_vall[] = {0, 0, 0, 0};
  float temp_vall[] = {0, 0, 0, 0};
  for(int i=0; i < NB_SCALES; i++){
    temp_adc_vall[i] = analogRead(LM35_PINS[i]);
    temp_vall[i] = temp_adc_vall[i] * 4.88; 
    temp_vall[i] = temp_vall[i]/10;
  }
  
  float weights[]={0,0,0,0};
  previous=actual;
  actual=0;
  for(int i=0; i<NB_SCALES; i++){
    Serial.print("Balance ");
    Serial.print(i+1);
    Serial.print(": ");
    weights[i] = scales[i].get_units();
    actual+=weights[i];
    Serial.print(weights[i], 1);
    //Serial.print("\t Temperature :");
    //Serial.print(temp_vall[i]);
    Serial.print("\t");
  }
  Serial.print("\t Poids :");
  Serial.println(actual);
  

//DEBUT DES CALCULS

  float C=529;        //longueur du rectangle en mm
  float c=324;        //largeur du rectangle en mm
  
  float x;            //position en x en mm
  float y;            //position en y en mm

  x=C*((weights[0]+weights[1])/actual);
  y=c*((weights[0] + weights[3])/actual);

  Serial.print(x);
  Serial.print("\t");
  Serial.println(y);
  delay(CLK);
}
