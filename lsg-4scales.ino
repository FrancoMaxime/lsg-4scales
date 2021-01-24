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

  This project uses mpflaga's excellent library : https://github.com/mpflaga/Arduino-MemoryFree
  and is released under a MIT LICENSE

  The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
  based load cell which should allow a user to measure everything from a few grams to tens of tons.

  scale 1:
   HX711 CLK      -> 2
   DAT            -> 3
   LM35 PINS      -> A1
   VCC            -> 5V
   GND            -> GND
   Calib Factor   -> -380
   Thermal factor -> 0.0083
   Temp° Calib    -> 21.45

  scale 2:
   HX711 CLK      -> 6
   DAT            -> 7
   LM35 PINS      -> A2
   VCC            -> 5V
   GND            -> GND
   Calib Factor   -> -380
   Thermal factor -> 0.0083
   Temp° Calib    -> 21.45

  scale 3:
   HX711 CLK      -> 4
   DAT            -> 5
   LM35 PINS      -> A3
   VCC            -> 5V
   GND            -> GND
   Calib Factor   -> -385
   Thermal factor -> 0.0083
   Temp° Calib    -> 21.45

  scale 4:
   HX711 CLK      -> 8
   DAT            -> 9
   LM35 PINS      -> A4
   VCC            -> 5V
   GND            -> GND
   Calib Factor   -> -390
   Thermal factor -> 0.0083
   Temp° Calib    -> 21.45

  The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#ifndef LSG_4SCALES
#define LSG_4SCALES
#define NB_SCALES 4
#define CLOCK 1000
#define WIDTH 540
#define HEIGHT 342
#define NB_PLACEMENTS 4
#define AVERAGE 3
#define LOADCELL_OFFSET 50682624
#define LOADCELL_DIVIDER 5895655
#define VERBOSE true
#define THERMAL_CORRECTION true
#define BLUETOOTH true
#define RX_PIN 10
#define TX_PIN 11
#endif

#include "HX711.h"

#if BLUETOOTH
#include <SoftwareSerial.h>
SoftwareSerial HC05(RX_PIN, TX_PIN);
#endif

template<typename T> void my_print(T data);
template<typename T> void my_println(T data );

template<typename T> void my_print(T data) {
  Serial.print(data);
#if BLUETOOTH
  HC05.print(data);
#endif
}
template<typename T> void my_println(T data) {
  Serial.println(data);
#if BLUETOOTH
  HC05.println(data);
#endif
}


typedef struct {
  short x_start, y_start;
  short x_end, y_end;
  float weight;
} area;

HX711 scales[4];
const float CALIBRATIONS_FACTORS[] = {-380, -380, -385, -390};
const float THERMAL_FACTORS[] = {0.0083, 0.0083, 0.0083, 0.0083};
const float TEMP_FACTORS[] = {21.45, 21.45, 21.45, 21.45};
const int LM35_PINS[] = {A1, A2, A3, A4};
const int SCALES_ID[][2] = {{3, 2}, {7, 6}, {5, 4}, {9, 8}};
const long ZERO_FACTORS[] = {0, 0, 0, 0};

area *placement;
int nb_areas = 0;
float x_previous = 0, y_previous = 0, x_actual = 0, y_actual = 0;
float previous = 0, actual = 0;

void setup() {
  Serial.begin(115200);
#if BLUETOOTH
  HC05.begin(9600);
#endif

  for (int i = 0; i < NB_SCALES; i++) {
    scales[i].begin(SCALES_ID[i][0], SCALES_ID[i][1]);
    scales[i].set_scale(CALIBRATIONS_FACTORS[i]);
    //scales[i].set_offset(LOADCELL_OFFSET);
    scales[i].tare();
    x_previous = WIDTH / 2;
    y_previous = HEIGHT / 2;
    previous = 0;
  }
  placement = new area[3];
  placement[0] = {0, 0, 180, 342, 0};
  placement[1] = {180, 0, 360, 342, 0};
  placement[2] = {360, 0, 540, 342, 0};
  nb_areas = 3;
}

void loop() {
  char temp = 0;
#if BLUETOOTH
  if (HC05.available()) {
    temp = HC05.read();
  }
#endif

  if (Serial.available() && temp == 0) {
    temp = Serial.read();
  }
  if (temp != 0) {
    bool modif = false;
    char changing[] = "Changing placement to ";
    switch (temp) {
      case 't':
        {
          modif = true;
          char msg0[] = "Doing a tare... ";
          my_print(msg0);
          for (int i = 0; i < NB_SCALES; i++) {
            scales[i].tare();
          }
          break;
        }
      case 'p':
        {
          char msg0[] = "Weight by Area.\n[";
          my_print(msg0);
          my_print(placement[0].weight);
          for (int i = 1; i < nb_areas; i++) {
            my_print(", ");
            my_print(placement[0].weight);
          }
          my_println("]");
          break;
        }
      /*case '?':
        Serial.println("Print all possible interaction with the serial port.");
        Serial.println("    ?-key: show this help.");
        Serial.println("    t-key: tare the LSG-4Scales.");
        Serial.println("    p-key: print the weight disposed on each area.");
        Serial.println("    0-key: Select a placement with one area representing all the LSG-4Scales.");
        Serial.println("    1-key: Select a placement that divide horizontally the LSG-4Scales in 3 areas..");
        Serial.println("    2-key: Select a placement that divide perpendicularly the LSG-4Scales in 3 areas.");
        Serial.println("    3-key: Select a placement that divide horizontally the LSG-4Scales in 4 areas and perpendicularly in 2 areas. (total : 8 areas)");
        Serial.println("    4-key: Select a placement that divide perpendicularly the LSG-4Scales in 2 areas. The top area is divided into 4 zones while the bottom zone is not more divided. (total 5 areas)");
        break;*/
      case '0':
        my_print(changing);
        my_print("0...");
        modif = true;
        delete[] placement;
        placement = new area[1];
        placement[0] = {0, 0, WIDTH, HEIGHT};
        nb_areas = 1;
        break;
      case '1':
        my_print(changing);
        my_print("1...");
        modif = true;
        delete[] placement;
        placement = new area[3];
        placement[0] = {0, 0, 180, 342, 0};
        placement[1] = {180, 0, 360, 342, 0};
        placement[2] = {360, 0, 540, 342, 0};
        nb_areas = 3;
        break;
      case '2':
        my_print(changing);
        my_print("2...");
        modif = true;
        delete[] placement;
        placement = new area[3];
        placement[0] = {0, 0, 540, 114};
        placement[1] = {0, 114, 540, 228};
        placement[2] = {0, 228, 540, 342};
        nb_areas = 3;
        break;
      case '3':
        my_print(changing);
        my_print("3...");
        modif = true;
        delete[] placement;
        placement = new area[8];
        placement[0] = {0, 0, 135, 171, 0};
        placement[1] = {135, 0, 270, 171, 0};
        placement[2] = {270, 0, 305, 171, 0};
        placement[3] = {305, 0, 540, 171, 0};
        placement[4] = {0, 171, 135, 342};
        placement[5] = {135, 171, 270, 342};
        placement[6] = {270, 171, 305, 342};
        placement[7] = {305, 171, 540, 342};
        nb_areas = 8;
        break;
      case '4':
        my_print(changing);
        my_print("4...");
        modif = true;
        delete[] placement;
        placement = new area[5];
        placement[0] = {0, 0, 135, 171, 0};
        placement[1] = {135, 0, 270, 171, 0};
        placement[2] = {270, 0, 305, 171, 0};
        placement[3] = {305, 0, 540, 171, 0};
        placement[4] = {0, 171, 540, 342, 0};
        nb_areas = 5;
        break;
    }
    if (modif) {
      x_previous = WIDTH / 2;
      y_previous = HEIGHT / 2;
      previous = 0;
      actual = 0;
      x_actual = 0;
      y_actual = 0;
      my_println("Done");
    }
  }

  //get the actual weight and temperature on each load cell.
  float weights[] = {0, 0, 0, 0};
  previous = actual;
  actual = 0;
  for (int i = 0; i < NB_SCALES; i++) {
    weights[i] = read_sensor(i);
    actual += weights[i];
    if (VERBOSE) {
      my_print("Scale ");
      my_print(i + 1);
      my_print(": ");
      my_print(weights[i]);
      my_print("\t");
    }
  }
  if (VERBOSE) {
    my_print("\t total Weight :");
    my_println(actual);
  }

  if (abs(actual - previous) > 1) {
    float tmp1 = 0;
    for (int i = 0; i < NB_SCALES; i++) {
      tmp1 += read_sensor(i);
    }

    if (abs(actual - tmp1) > 1) {
      actual = 0;
      my_println("Taking a last time the weight cause it is not stable.");
      for (int i = 0; i < NB_SCALES; i++) {
        weights[i] = read_sensor(i);
        actual += weights[i];
      }
    }
  }

  if (abs(actual - previous) > 1) {
    float tmp = 0;
    for (int i = 0; i < NB_SCALES; i++) {
      weights[i] = scales[i].get_units(AVERAGE);
      tmp += weights[i];
    }

    //Determine the position of the actual gravity center of the scale.
    x_actual = WIDTH * ((weights[1] + weights[2]) / actual);
    y_actual = HEIGHT * ((weights[2] + weights[3]) / actual);
    if (VERBOSE) {
      my_print("New gravity center.\n[");
      my_print(x_actual);
      my_print(", ");
      my_print(y_actual);
      my_println("]");
    }

    //Determine where the weight was dropped on the scale.
    float tmp_weight = actual - previous;
    float x_weight = (((x_actual - x_previous) * previous) / tmp_weight) + x_actual;
    float y_weight = (((y_actual - y_previous) * previous) / tmp_weight) + y_actual;
    if (VERBOSE) {
      my_print("Position of the weight.\n[");
      my_print(x_weight);
      my_print(", ");
      my_print(y_weight);
      my_println("]");
    }

    //Determine in with area the weight was placed.
    if (VERBOSE)
      my_print("Weight by Area.\n[");
    for (int i = 0; i < nb_areas; i++) {
      if (placement[i].x_start <= x_weight && x_weight < placement[i].x_end && placement[i].y_start <= y_weight && y_weight < placement[i].y_end) {
        placement[i].weight += tmp_weight;
      }
      if (VERBOSE) {
        my_print(placement[i].weight);
        if (i < nb_areas - 1 )
          my_print(", ");
      }
    }
    if (VERBOSE)
      my_println("]");
    x_previous = x_actual;
    y_previous = y_actual;
    previous = actual;
  }
  delay(CLOCK);
}

float read_sensor(int id_sensor) {
  float value = scales[id_sensor].get_units(AVERAGE);
  if (THERMAL_CORRECTION) {
    float temp_adc_vall = analogRead(LM35_PINS[id_sensor]);
    float temp_vall = temp_adc_vall * 4.88;
    temp_vall = temp_vall / 10;
    value = value - (THERMAL_FACTORS[id_sensor] * (temp_vall - TEMP_FACTORS[id_sensor]));
  }
  return value;
}
