#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include "SerialPort.h"
#include "Volume.h"

#define DEFAULT_PORT "/dev/ttyACM0"

//helper function returns a percentage
inline int toPercent(int num, int max){
    float fNum = static_cast<float>(num);
    fNum /= max;
    fNum *= 100;
    return static_cast<int>(fNum);
}

int main(int argc, char **argv){
    //open serial port
    SerialPort mySerial = (argc > 1) ? argv[1] : DEFAULT_PORT;

    //declare bitfield and volume
    TenBitfield serialVals;
    Volume myVolume;

    //forever read the serial and adjust the volumes
    for(;;){
        //read serial
        serialVals = mySerial.readInt32();

        //set application volume
        int app1 = myVolume.getApplicationNum("spotify");
        printf("%d%%\n", toPercent(serialVals.a1, 1023));
        myVolume.setVolume(app1, toPercent(serialVals.a1, 1023));        
    }

    return 0;
}