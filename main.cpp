#include <iostream>
#include <stdio.h>
#include <bitset>

#include "SerialPort.h"

struct myBitfield{
    myBitfield operator= (const unsigned int& rhs){
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

    //assumes incoming has EIGHT BYTES
    unsigned int a1 : 10; //0-1023
    unsigned int a2 : 10;
    unsigned int a3 : 10;
};

int main(int argc, char **argv){
    std::cout << sizeof(myBitfield) << std::endl;

    myBitfield mb;
    //unsigned int myInt = 1023;
    //unsigned int myInt = 0x3fffffff; //0b001*30
    //unsigned int myInt = 0b00111111110111111111101111111111;
                        //0b00 1111111101 1111111110 1111111111;
                        //0    1021       1022       1023
    unsigned int myInt = 0b00001110111011000010010000011001;
    //238 777 25

    //LEFTMOST PART
    printf("%d\n", myInt >> 20);

    //MIDDLE PART
    printf("%d\n", (myInt << 12) >> 22);

    //RIGHTMOST PART
    printf("%d\n", (myInt << 22) >> 22);

    //return 98;

    mb = myInt;
    
    std::cout << mb.a1 << std::endl;
    std::cout << mb.a2 << std::endl;
    std::cout << mb.a3 << std::endl;

    return 99;
    //SerialPort mySerial = "/dev/ttyACM1";

    //for(;;)
     //   std::cout << mySerial.readInt() << std::endl;
    //return 0;
}