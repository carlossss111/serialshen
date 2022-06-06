#define F_CPU 16000000UL    //clock speed in Hz
#define __AVR_ATmega328P__  //vscode highlighting

#include <stdint.h>
#include <avr/io.h>         //io ports
#include <util/delay.h>     //_delay_ms() function

//configure the baud rate, transmit and formatting on startup
void configureSerial(uint32_t baud){
    /* CALCULATE UBRR REGISTER VALUE - page 146
    *  Uses the following formula:
    *  ubrr = ( oscilator freq / (16 * baud rate) ) - 1
    */
    uint32_t ubrr;
    ubrr = (F_CPU / (16 * baud)) - 1;

    /* SET BAUD RATE - page 162
    *  UBRR_High contains the 4 most significant bits of ubrr and is 12 bits long
    *  UBRR_Low contains the 8 least significant bits, the rest is truncated
    */
    UBRR0H = ubrr >> 8; 
    UBRR0L = ubrr;

    /* ENABLE TRANSMITTER - page 160
    *  Enable bit 3 (TXEN0) in the UCSRnB register
    */
    UCSR0B = (1 << TXEN0);

    /* FORMAT FRAME - page 147 and 161
    *  Want to be able to send 8 bit bytes.
    *  Therefore enable bit 2 (UCSZ01) and bit 1 (UCSZ00)
    */
    UCSR0C = (1 << UCSZ01 | 1 << UCSZ00);
}

//send a string through the serial port
void sendSerial(const char * str){
    int i = 0;
    /* SEND FRAME - page 159
    *  Loop through the string and wait while the transmitter is busy:
    *    - "1 << UDRE0" moves a bit to the position 5 (00100000)
    *    - "UCSR0A & .." returns whether UCSR0A is on or off
    *    - "!.." makes it wait while it is off
    *  Send 7 bits to the data register UDR0 (an ASCII character)
    */
    while(str[i]){
        while ( !(UCSR0A & (1 << UDRE0)) );
        UDR0 = str[i];
        i++;
    }
}

int main(int argc, char **argv){

    //configure the serial port
    configureSerial(9600);

    for(;;){
        //send a serial message
        sendSerial("Hello World\n");

        //busy waiting
        _delay_ms(500);
    }

    
    return 0;
}