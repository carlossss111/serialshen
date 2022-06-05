#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include "SerialPort.h"

struct tenBitfield{
    //split a 32bit integer into 10 bits each.
    tenBitfield operator= (const uint32_t rhs){
        //LEFTMOST - first ten bits after "0b00"
        //right shifts 20 places to only keep the 12 bits that were originally leftmost
        a1 = rhs >> 20;
        //MIDDLE - next ten bits
        //left shifts 12 places to remove left bits then shift remaining bits to the far right
        a2 = (rhs << 12) >> 22;
        //RIGHT - last ten bits
        //left shifts 22 places to remove all left bits then shift remaining bits to the far right
        a3 = (rhs << 22) >> 22;
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