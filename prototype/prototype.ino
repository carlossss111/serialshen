#include <stdint.h>
#include <stdlib.h>

#define LOOP_INTERVAL_MS 500
#define PIN_1 A0
#define PIN_2 A2
#define PIN_3 A4

//return 4 byte (32 bit) unsigned int in the form 0b00[lBits][mBits][rBits]
uint32_t constructMessage(int lBits, int mBits, int rBits){
  uint32_t mOut;
  mOut = lBits << 10; //leftshift 10 leftmost bits
  mOut = (mOut | mBits) << 10; //OR the middle bits in and leftshift them
  mOut = mOut | rBits; //OR the right bits in
  return mOut;
}

//setup pinmodes and serial
void setup(){
  pinMode(PIN_1, INPUT);
  pinMode(PIN_2, INPUT);
  pinMode(PIN_3, INPUT);
  Serial.begin(9600);
}

//read the analog pins, construct a message with them and send it
void loop(){
  //read all of pins. the max is 1023 (10 bits)
  int a1 = analogRead(PIN_1);
  int a2 = analogRead(PIN_2);
  int a3 = analogRead(PIN_3);

  //construct a 4 byte message,
  //send 5 bytes (4 byte int + newline character)
  Serial.println(constructMessage(a1, a2, a3));
  
  //wait
  delay(LOOP_INTERVAL_MS);
}
