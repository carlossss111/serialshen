#include <iostream>

#include "SerialPort.h"

int main(int argc, char **argv){
    SerialPort mySerial = "/dev/ttyACM1";

    for(;;)
        std::cout << mySerial.readInt() << std::endl;
}