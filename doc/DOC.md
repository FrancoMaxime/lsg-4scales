# Documentation for the LSG-4Scales
## Presentation
[Presentation of LSG-4Scales](https://github.com/FrancoMaxime/lsg-4scales/blob/main/README.md)

## #define Preprocessor
- LSG_4SCALES: To avoid recursive inclusion of the project
- NB_SCALES: The number of load cells use in the project
- CLOCK: Break time in millisecond after a measure
- WIDTH: Width of the LSG-4Scales.
- HEIGHT: Height of the LSG-4Scales.
- NB_PLACEMENTS: Number of different placements for the LSG-4Scales.
- AVERAGE: Average of multiple readings instead of a single reading for a measure
- VERBOSE: Set to 1 if the data should be output in the terminal
- THERMAL_CORRECTION: Set to 1 if the measure should be corrected by the thermal factor
- BLUETOOTH: Set to 1, if the LSG-4Scales contains a Bluetooth
- RX_PIN: Bluetooth's RX Pin
- TX_PIN: Bluetooth's TX Pin

## Structure
```c
typedef struct {
  short x_start, y_start;
  short x_end, y_end;
  float weight;
} area;
```
Represent an area on the LSG-4Scales. The area has a starting point (x_start, y_start), a ending point (x_end, y_end) and a weight. A area is a rectangle and the weight is the measure of the total weight inside.

## Constants

- CALIBRATIONS_FACTORS[]: array containing the calibration factor of each load cell
- THERMAL_FACTORS[]: array containing the thermal factor of each load cell
- TEMP_FACTORS[]: array containing the temperature's factor of each load cell
- LM35_PINS[]: array containing the pin for each temperature sensor, related to each load cell
- SCALES_ID[][2]: array of a 2-tuple. There is a tuple for each load cell containing the DAT pin and the CLK pin
- ZERO_FACTORS[]: If there is a constant weight deposited of the LSG-4Scales, you can define a tare weight that will be subtracted (related to each load cell)


## Global Variable
- area *placement: pointer to the array of areas containing all areas of the current placement
- nb_areas: Number of areas for the current placement
- x_previous, y_previous: the position of the previous gravity center of the LSG-4Scales
- x_actual, y_actual: the position of the actual gravity center of the LSG-4Scales
- previous: the total weight of the previous measure
- actual: the actual weight deposited on the LSG-4Scales

## Objects
- scales[4]: The array containing the pointer to the 4 HX711 representing the loads cells
- HC05: the pointer to the object representing the Bluetooth device

## Methods
- template<typename T> void my_print(T data): Template for printing data on the Serial and/or the bluetooth serial
- template<typename T> void my_println(T data): Template for printing data with a line break on the Serial and/or the bluetooth serial
- float read_sensor(int id_sensor): read the weight on the specific id_sensor scale

## Hardware support
- [ATmega328](https://en.wikipedia.org/wiki/ATmega328): Arduino Uno


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.


## License
[MIT](https://choosealicense.com/licenses/mit/)
