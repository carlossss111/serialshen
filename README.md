# Serial Shenanigans

I wanted to see how you could receive a serial from an arduino (or another device) and use it to do things in the operating system with C/C++. The end goal is to make hardware volume dials for different processes. (very W.I.P.)

## Compilation and running
```
g++ main.cpp SerialPort.cpp && ./a.out
```

## Bitwise Maths
The embedded program takes the input from three different potentiometers and sends those values through the serial port as a single 32 bit integer.

### constructMessage() function
1. The output variable mOut is zero'd and has the uint32_t type from \<stdint.h\> so that it is portable.
1. Each potentiometer value is always between 0-1032 which can be written with 10 bits. mOut is assigned to the first of these values.
1. The bits are shifted 10 places to the left, leaving 10 zero bits in the places on the right.
1. The bits from the second potentiometer value are OR'd onto mOut, filling the 10 right-most bits.
1. The previous 2 steps are repeated for the last value and mOut is returned.

Example 42 and 112:
```
write 42 as binary
> 0000000000 0000101010
leftshift by 10 digits
> 0000101010 0000000000
OR in the next value 112 (0000000000 0001110000)
> 0000101010 0001110000
the resulting value contains both values either side (10 digits each)
```

### tenBitfield() assignment operator
1. a1, a2 and a3 are bitfield variables that store 10 bits each. The bitsIn variable contains the 32 bit integer recieved through the serial port.
1. The 10 leftmost bits are rightshifted 20 places so that they become the rightmost bits and are therefore the bits that will be stored in a1.
1. The 10 middle bits are rightshifted 10 places in the same way.
1. The 10 rightmost bits are already the least significant so can be stored without bitshifting.

Example 42 and 112:
```
get 42 and 112 as a binary pair (= 43120)
> 0000101010 0001110000
'a3' can directly store 112 (0001110000) since it is the ten least significant bits
> a3 = 0001110000
rightshift the original value by 10 digits
> 0000000000 0000101010
'a2' can now store 42 (0000101010) since it is now least significant
> a2 = 0000101010
```
Did I need to squash 3 values into 4 bytes and complicate things? no. Did I do it anyway? yes it was interesting :-).

## Code
__SerialPort.cpp/h__ has a constructor that opens the serial port as a file and configures it. This is the most reusable part of the repo. The configure() function has specifics like:
* Runs on 9600 baud.
* Messages from the port must end with a newline character to be received (called "canonical mode").
* Waits for up to 5 seconds for a message before timing out.
* A bunch of useless extra stuff like parity bits are disabled.

__readSerial.c__ is just SerialPort.cpp/h but in plain C. This is unused but I'm keeping it here for interest.

__main.cpp__ contains the main function.

__prototype.ino__ contains the arduino code to read 3 potentiometers and transmit each of their values as a single 4 byte integer.

## Circuit
This assumes a generic arduino device connected to a USB port.
* Place 3 potentiometers in parallel.
* Connect all of them to 5V and GND, then each to A0, A2 and A4.
* Add a useless LED somewhere because it'll look cool.