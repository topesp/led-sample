# ESP32 LED Sample Project

A simple ESP32 project demonstrating GPIO control with an LED and a switch.

## Hardware Requirements

- ESP32 development board
- LED
- Push button/switch
- Appropriate resistors
- Jumper wires

## Pin Configuration

- LED: GPIO22
- Switch: GPIO21

## Setup Instructions

1. Install ESP-IDF
   ```bash
   # Make sure you have ESP-IDF installed and properly configured
   ```

2. Clone the repository
   ```bash
   git clone https://github.com/topesp/led-sample.git
   cd led-sample
   ```

3. Build the project
   ```bash
   idf.py build
   ```

4. Flash to your ESP32
   ```bash
   idf.py -p [PORT] flash
   ```
   Replace [PORT] with your ESP32's serial port (e.g., /dev/ttyUSB0 on Linux or /dev/cu.SLAB_USBtoUART on macOS)

## Circuit Connection

1. Connect LED positive (longer leg) to GPIO22 through a current-limiting resistor
2. Connect LED negative (shorter leg) to GND
3. Connect one terminal of the switch to GPIO21
4. Connect the other terminal of the switch to GND

## Usage

- When the switch is pressed (connected to GND), the LED will turn on
- When the switch is released, the LED will turn off
- The LED state is inversely related to the switch state due to the logic in the code

## Project Structure

- `main/`
  - `main.c` - Main application code
  - `CMakeLists.txt` - Component make file
- `CMakeLists.txt` - Project make file
- `.gitignore` - Git ignore file
- `README.md` - This file

## License

This project is open source and available under the MIT License.
