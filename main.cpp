#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include "SerialPort.h"

struct tenBitfield{
    //split a 32bit integer into 10 bits each, 0b00[a1*ten][a2*ten][a3*ten]
    tenBitfield operator= (const uint32_t bitsIn){
        a1 = bitsIn >> 20; //LEFTMOST - first ten bits after "0b00"
        a2 = bitsIn >> 10; //MIDDLE - next ten bits
        a3 = bitsIn; //RIGHT - last ten bits
        return *this;
    }   

    //assumes incoming has FOUR BYTES
    unsigned int a1 : 10; //0-1023
    unsigned int a2 : 10;
    unsigned int a3 : 10;
};

int main(int argc, char **argv){
    //open serial port
    SerialPort mySerial = "/dev/ttyACM0";

    //read the 4 bytes and get the seperated information
    tenBitfield numField;
    for(;;){
        uint32_t foundNum = mySerial.readInt32();
        numField = foundNum;
        std::cout << "RAW: " << foundNum;
        std::cout << " SIZE: " << sizeof(foundNum) << std::endl;
        std::cout << numField.a1 << ", " << numField.a2;
        std::cout << ", " << numField.a3 << std::endl;
    }
}