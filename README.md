# LSG-4Scales

LSG-4Scales is an Arduino sketch for analysing the variation of the gravity center of a scale with 4 load cells placed at the corners of a rectangle. The 4Scale can be divided in different area to get the sum of the weight inside each area (See section: Areas of the LSG-4Scales).

## Installation

- Use the library manager [Arduino](https://www.arduino.cc/en/software) to install [HX711](https://github.com/bogde/HX711)
- Use the Boards manager [Arduino](https://www.arduino.cc/en/software) to install Atmel AVR Xplained-minis

## Hardware support
- [ATmega328](https://en.wikipedia.org/wiki/ATmega328): Arduino Uno

## Usage

- T key: Taring the 4Scale
- 1 key: Divide horizontally the 4Scale in 3 areas.
- 2 key: Divide perpendicularly the 4Scale in 3 areas.
- 3 key: Divide horizontally the 4Scale in 4 areas and perpendicularly in 2 areas. (total : 8 areas)
- 4 key: Divide perpendicularly the 4Scale in 2 areas. The top area is divided into 4 zones while the bottom zone is not more divided. (total 5 areas)

## Areas of the LSG-4Scales
- 1 Key: ![Image text](https://i.ibb.co/3yZvxKq/area1.png)
- 2 Key: ![Image text](https://i.ibb.co/64CXgdw/area2.png)
- 3 Key: ![Image text](https://i.ibb.co/wwD9MCz/area3.png)
- 4 Key: ![Image text](https://i.ibb.co/QmWrXKV/area4.png)

## Documentation 
[Documentation](https://github.com/FrancoMaxime/lsg-4scales/blob/main/doc/DOC.md)

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)
