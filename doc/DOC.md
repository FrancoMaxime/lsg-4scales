# Documentation for the LSG-4Scales
[Presentation of LSG-4Scales](https://github.com/FrancoMaxime/lsg-4scales/blob/main/README.md)


## Constants

- CALIBRATIONS_FACTORS[] : array constaining the calibration factor of each load cell
- LM35_PINS[] : array constaining the pin for each temperature sensor, related to each load cell.
- SCALES_ID[][2] : array of a 2-tuple. There is a tuple for each load cell containing the DAT pin and the CLK pin.
- NB_SCALES : The number of load cells use in the project.
- ZERO_FACTORS[]: If there is a constant weight deposited of the LSG-4Scales, you can define a tare weight that will be subtracted (related to each load cell).
- CLOCK : Break time in millisecond after a measure.
- WIDTH : Width of the LSG-4Scales.
- HEIGHT: Height of the LSG-4Scales.
- NB_PLACEMENTS: Number of different placements for the LSG-4Scales.
- NB_AREAS: Number of areas for each placement
- AREAS[][4]: Array of 4-tuple, each tuple corresponds to an area, the 2 first integers correspond to the starting point of the area, and the two lasts to the ending point.


## Configuration
- CHOSEN_PLACEMENT : corresponds to the chosen placement at the start of the LSG-4Scales.
- START_PLACEMENT: corresponds to the start of the are's ID inside the array AREAS[][4].
- WEIGHT_AREA[8]: contains the weight inside each area. Must be set to 0 at the start of the LSG-4Scale, or after a tare.

## Others global variables
- x_previous, y_previous: the position of the previous gravity center of the LSG-4Scales.
- x_actual, y_actual: the position of the actual gravity center of the LSG-4Scales.
- previous: the total weight of the previous measure.
- actual: the actual weight deposited on the LSG-4Scales.

## Hardware support
- [ATmega328](https://en.wikipedia.org/wiki/ATmega328): Arduino Uno


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.


## License
[MIT](https://choosealicense.com/licenses/mit/)
