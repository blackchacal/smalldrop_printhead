# SmallDrop - Print Head FW

This repository contains the firmware for the SmallDrop system's print head. It is based on an Arduino Nano Rev3 Board.

## Architecture

The print head was created around the concept of a syringe pump system. A stepper motor is connected to a lead screw that moves the syring pyston when turning. The stepper motor provides some degree of precision control during the extrusion process. 

### Print volumes and syringes
The print head should allow to use three different sizes of common plastic syringes, 2 ml, 5 ml and 10 ml.

### Motor & Lead Screw
For this project, the stepper motor chosen was a [NEMA17 17HS4401](https://www.amazon.de/-/en/Twotrees-Stepper-Motor-Printer-Engines/dp/B07SQNYZDY/) from Twotrees. It is a bipolar stepper motor that has ~42 Ncm torque, 200 steps/rev which means a 1.8º step.

The lead screw will be an 8 mm diameter screw. The coupling is direct using a 5 to 8 mm flexible motor shaft coupler, generally used on rep-rap 3D printers.

## Development

The firmware was developed using PlatformIO on VS Code.

## License

This project is licensed under the [MIT License](LICENSE).