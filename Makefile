local_compile:
	g++ -o bin/local.bin localsrc/main.cpp localsrc/SerialPort.cpp localsrc/Volume.cpp

local: local_compile
	./bin/local.bin

local_mem: local_compile
	valgrind ./bin/local.bin

embedded_compile:
	avr-gcc -Os -mmcu=atmega328p -o bin/embedded.bin embeddedsrc/embedded.c
	avr-objcopy -O ihex -R .eeprom bin/embedded.bin bin/embedded.hex

embedded: embedded_compile
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:bin/embedded.hex
	rm bin/embedded.hex
