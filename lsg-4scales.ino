/*
  Sketch for a rectangle 4 load cells scale
  By: Maxime Franco, Simon De Jaeger
  ULiège
  Date: January 17th, 2021
  License: MIT license

  The goal of this sketch is to determine where the weight is put by analysing the variation of the gravity center of the different weights deposited on the scale. See the calibration sketch to get the calibration_factor for your
  specific load cell setup.

  This project uses bogde's excellent library: https://github.com/bogde/HX711
  bogde's library is released under a GNU GENERAL PUBLIC LICENSE

  The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
  based load cell which should allow a user to measure everything from a few grams to tens of tons.

  scale 1:
   HX711 CLK    -> 2
   DAT          -> 3
   LM35 PINS    -> A1
   VCC          -> 5V
   GND          -> GND
   Calib Factor -> -380

  scale 2:
   HX711 CLK    -> 6
   DAT          -> 7
   LM35 PINS    -> A2
   VCC          -> 5V
   GND          -> GND
   Calib Factor -> -380

  scale 3:
   HX711 CLK    -> 4
   DAT          -> 5
   LM35 PINS    -> A3
   VCC          -> 5V
   GND          -> GND
   Calib Factor -> -385

  scale 4:
   HX711 CLK    -> 8
   DAT          -> 9
   LM35 PINS    -> A4
   VCC          -> 5V
   GND          -> GND
   Calib Factor -> -390

  The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include "HX711.h"

HX711 scales[4];
const float CALIBRATIONS_FACTORS[] = {-380, -380, -385, -390};
const int LM35_PINS[] = {A1, A2, A3, A4};
const int SCALES_ID[][2] = {{3, 2}, {7, 6}, {5, 4}, {9, 8}};
const int NB_SCALES = 4;
const long ZERO_FACTORS[] = {0, 0, 0, 0};
const long LOADCELL_OFFSET = 50682624;
const long LOADCELL_DIVIDER = 5895655;
const int CLOCK = 5000;
const int WIDTH = 540;
const int HEIGHT = 342;
const int NB_PLACEMENTS = 4;
const int NB_AREAS[] = {3, 3, 8, 5};
const int AREAS[][4] = {{0, 0, 180, 342}, {180, 0, 360, 342}, {360, 0, 540, 342}, {0, 0, 540, 114}, {0, 114, 540, 228}, {0, 228, 540, 342}, {0, 0, 135, 171}, {135, 0, 270, 171}, {270, 0, 305, 171}, {305, 0, 540, 171}, {0, 171, 135, 342}, {135, 171, 270, 342}, {270, 171, 305, 342}, {305, 171, 540, 342}, {0, 0, 135, 171}, {135, 0, 270, 171}, {270, 0, 305, 171}, {305, 0, 540, 171}, {0, 171, 540, 342}};

int CHOSEN_PLACEMENT = 0;
int START_PLACEMENT = 0;
float WEIGHT_AREA[] = {0, 0, 0, 0, 0, 0, 0, 0};

float x_previous = 0, y_previous = 0, x_actual = 0, y_actual = 0;
float previous = 0, actual = 0;
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < NB_SCALES; i++) {
    scales[i].begin(SCALES_ID[i][0], SCALES_ID[i][1]);
    scales[i].set_scale(CALIBRATIONS_FACTORS[i]);
    //scales[i].set_offset(LOADCELL_OFFSET);
    scales[i].tare();
    x_previous = WIDTH / 2;
    y_previous = HEIGHT / 2;
    previous = 0;
  }
}

void loop() {
  if (Serial.available()) {
    char temp = Serial.read();
    bool modif = false;
    switch (temp) {
      case 't':
        modif = true;
        Serial.print("Doing a tare... ");
        for (int i = 0; i < NB_SCALES; i++) {
          scales[i].tare();
        }
        Serial.println("Done");
        break;
      case '1':
        modif = true;
        CHOSEN_PLACEMENT = 0;
        break;
      case '2':
        modif = true;
        CHOSEN_PLACEMENT = 1;
        break;
      case '3':
        modif = true;
        CHOSEN_PLACEMENT = 2;
        break;
      case '4':
        modif = true;
        CHOSEN_PLACEMENT = 3;
        break;
    }
    if (modif) {
      Serial.print("Changing area... ");
      START_PLACEMENT = 0;
      for (int i = 0; i < CHOSEN_PLACEMENT; i++) {
        START_PLACEMENT += NB_AREAS[i];
      }
      for (int i = 0; i < 8; i++) {
        WEIGHT_AREA[i] = 0;
      }
      x_previous = WIDTH / 2;
      y_previous = HEIGHT / 2;
      previous = 0;
    }
  }

  //get the actual weight and temperature on each load cell.
  float weights[] = {0, 0, 0, 0};
  previous = actual;
  actual = 0;
  float max_weight = 0;
  int temp_adc_vall[] = {0, 0, 0, 0};
  float temp_vall[] = {0, 0, 0, 0};
  for (int i = 0; i < NB_SCALES; i++) {
    temp_adc_vall[i] = analogRead(LM35_PINS[i]);
    temp_vall[i] = temp_adc_vall[i] * 4.88;
    temp_vall[i] = temp_vall[i] / 10;

    weights[i] = scales[i].get_units(10);
    if (max_weight < weights[i]) {
      max_weight = weights[i];
    }
    actual += weights[i];
    Serial.print("Scale ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(weights[i], 1);
    Serial.print("\t");
  }
  Serial.print("\t total Weight :");
  Serial.println(actual);

  if (abs(actual - previous) > 1) {
    //Determine the position of the actual gravity center of the scale.
    /*if (max_weight == weights[0] || max_weight == weights[2]){
      x_actual = WIDTH * ((weights[2] + weights[3]) / actual);
      y_actual = HEIGHT * ((weights[1] + weights[2]) / actual);
      }
      else{
      x_actual = WIDTH * ((weights[0] + weights[1]) / actual);
      y_actual = HEIGHT * ((weights[0] + weights[3]) / actual);
      }
    */
    x_actual = WIDTH * ((weights[1] + weights[2]) / actual);
    y_actual = HEIGHT * ((weights[2] + weights[3]) / actual);
    Serial.print("New gravity center.\n[");
    Serial.print(x_actual);
    Serial.print(", ");
    Serial.print(y_actual);
    Serial.println("]");

    //Determine where the weight was dropped on the scale.
    float tmp_weight = actual - previous;
    float x_weight = (((x_actual - x_previous) * previous) / tmp_weight) + x_actual;
    float y_weight = (((y_actual - y_previous) * previous) / tmp_weight) + y_actual;
    Serial.print("Position of the weight.\n[");
    Serial.print(x_weight);
    Serial.print(", ");
    Serial.print(y_weight);
    Serial.println("]");

    //Determine in with area the weight was placed.
    Serial.print("Weight by Area.\n[");
    for (int i = 0; i < NB_AREAS[CHOSEN_PLACEMENT]; i++) {
      if (AREAS[START_PLACEMENT + i][0] <= x_weight && x_weight < AREAS[START_PLACEMENT + i][2] && AREAS[START_PLACEMENT + i][1] <= y_weight && y_weight < AREAS[START_PLACEMENT + i][3]) {
        WEIGHT_AREA[i] += tmp_weight;
      }
      Serial.print(WEIGHT_AREA[i]);
      Serial.print(", ");
    }
    Serial.println("]");
    x_previous = x_actual;
    y_previous = y_actual;
  }

  previous = actual;
  delay(CLOCK);
}
