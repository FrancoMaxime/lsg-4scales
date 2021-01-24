# LSG-4Scales
LSG-4Scales is an Arduino sketch for analysing the variation of the gravity center of a scale with 4 load cells placed at the corners of a rectangle. The LSG-4Scales can be subdivided in areas, allowing to calculate the total weight placed on each of them.

## Installation
- Use the libraries manager [Arduino](https://www.arduino.cc/en/software) to install [HX711](https://github.com/bogde/HX711)
- Use the boards manager [Arduino](https://www.arduino.cc/en/software) to install Atmel AVR Xplained-minis

## Hardware support
- [ATmega328](https://en.wikipedia.org/wiki/ATmega328): Arduino Uno

## Calibration
You can use the [LSG-4Scales-calibration](https://github.com/FrancoMaxime/lsg-4scales-calibration) project to calibrate your LSG-4Scales. The way to do it is explained inside the README of the project. 

## Thermal correction
A thermal correction is possible if you have a sensor connected to each load cells. You should calibrate it by using the [LSG-4Scales-calibration](https://github.com/FrancoMaxime/lsg-4scales-calibration) to get the thermal factors with the calibration temperature.

## Usage
- t key: Tare the LSG-4Scales.
- p key: Print the weight inside each area.
- 0 key: Define the placement as a single area for the whole scale.
- 1 key: Divide horizontally the LSG-4Scales in 3 areas.
- 2 key: Divide perpendicularly the LSG-4Scales in 3 areas.
- 3 key: Divide horizontally the LSG-4Scales in 4 areas and perpendicularly in 2 areas. (total : 8 areas)
- 4 key: Divide perpendicularly the LSG-4Scales in 2 areas. The top area is divided into 4 zones while the bottom zone is not more divided. (total 5 areas)

## Areas of the LSG-4Scales
- 0 key: ![Image text](https://i.ibb.co/CnV8vD2/area0.png)
- 1 Key: ![Image text](https://i.ibb.co/3yZvxKq/area1.png)
- 2 Key: ![Image text](https://i.ibb.co/64CXgdw/area2.png)
- 3 Key: ![Image text](https://i.ibb.co/wwD9MCz/area3.png)
- 4 Key: ![Image text](https://i.ibb.co/QmWrXKV/area4.png)

## Documentation 
You can find the documentation inside the [DOC](https://github.com/FrancoMaxime/lsg-4scales/blob/main/doc/DOC.md) file.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)
