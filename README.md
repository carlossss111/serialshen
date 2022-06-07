# Serial Shenanigans

Change the volume of three different applications on a linux machine using hardware dials. This is done by using an ATMEGA328P and reading voltages when the dials are turned.

## Compilation
Run both of the following commands from the project root directory.

### Embedded
The makefile will compile with avr-gcc then flash the data onto a plugged in ATMEGA328P. Similiar devices would probably work with minor alterations to the code.
```
make embedded
```

### Local (Linux)
The makefile will compile with g++.
```
make local_compile
```

## Circuit
The easiest way to recreate this circuit is with an Arduino Uno, since it already connects the ATMEGA328P chip to a USB port and power. That said, there is no Arduino specific code and a properly powered ATMEGA328P with a 16MHz clock and a serial connection should work just as well. Assuming either of these, the following circuit is simple:

* Place 3 potentiometers in parallel.
* Connect all of them to 5V and GND, then each to ADC0, ADC2 and ADC4 (check the pinout diagrams).
* Connect a 100μF capacitor between 5V and GND.

Connect the ATMEGA328P to the PC with a USB (serial) port. 

## Usage
After following the steps above, run _local.bin_ in the project's _bin_ directory while the embedded device is plugged into a USB (serial) port.

Turn the potentiometers to adjust the volumes of spotify, discord and firefox (placeholders defined in main.cpp).

## Dependencies

## Embedded
```
apt install avr-libc avrdude binutils-avr gcc-avr
```
* _avr-libc_ provides embedded compatible C standard libraries
* _avrdude_ flashes the binaries to the embedded chips
* _binuitls-avr_ has utilities for compiling/flashing (e.g. avr-objcopy)
* _gcc-avr_ is the embedded C compiler


### Local (Linux)
```
apt install libpulse-dev
```
* _pactl_, which is compiled and linked with _libpulse_ (ver. 13.99.00), can change the volume levels using pulse audio commands. (This is often pre-installed).

## Code Structure

### Embedded

_embedded.c_ is the only file flashed to the ATMEGA328P. It has 2 main groups of functions, one group configures and writes to the UART serial port, the other group configures and reads from the Analog to Digital Converter (ADC).

Both the serial port and the ADC are configured and used by manipulating special registers, documented by inline comments and in the [online datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf). The comments should make the file relatively straight-forward to read once the bitwise maths is understood.

In the main loop, the ADC is read 3 times using different pins, each returning a value between 0 - 1023 (10 bits). All 3 values are sent as a single 30 bit int (with 2 extra redundant bits) and with a newline character to the local PC through the serial connection.

### Local

The _SerialPort.cpp/h_ files are general-purpose and can read strings from the serial port sent once they are terminated with a newline (called Cannoical Mode). The read functions return strings or 32 bit integers to the caller.

The _Volume.cpp/h_ files are specific and use BASH to get and send volume commands to pulse-audio.

_main.cpp_ uses both of the above files to repeatedly receive the values from the serial port, decode them and use them to adjust volumes.

The above code could easily be changed or extended for the serial values to do different things, related or unrelated to audio.

### Datasheet
https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

---
Daniel Robinson 06/2022