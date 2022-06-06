#define F_CPU 16000000UL    //clock speed in Hz
#define __AVR_ATmega328P__  //vscode highlighting

#include <stdio.h>
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

//send a byte through the serial port
int sendByte(const char byteOut){    
    /* SEND FRAME - page 159
    *  Loop through the string and wait while the transmitter is busy:
    *    - "1 << UDRE0" moves a bit to the position 5 (00100000)
    *    - "UCSR0A & .." returns whether UCSR0A is on or off
    *    - "!.." makes it wait while it is off
    *  Send 7 bits to the data register UDR0 (an ASCII character)
    */
    while ( !(UCSR0A & (1 << UDRE0)) );
    UDR0 = byteOut;
}

//configure stdout to point to the serial port
int sput(char c, FILE *_) { return !sendByte(c); }
void configurePrintf(FILE *fp){
    fdev_setup_stream(fp, sput, NULL, _FDEV_SETUP_WRITE);
    stdout = fp;
}

//configure the analog to digital voltage converter
void configureADC(){
    /* POWER UP ADC - page 38
    *  Clear the the bit in the power reduction register PRR.
    */
    PRR &= ~(1 << PRADC);

    /* SET REFERENCE VOLTAGE "AREF" TO Vcc-5V AND SET PIN TO READ - page 217
    *  Set the leftmost bits to '01'
    *  Select the pin to read (ADC4)
    */
    ADMUX &= ~(1 << REFS1);
    ADMUX |= 1 << REFS0;
    ADMUX |= ADC4D;

    /* SCALE RESULT - page 217
    *  ADC needs an input clock of between 50kHz to 200kH to handle 10 bit readings,
    *  divide the clockspeed by 8 with the following
    */
    ADCSRA |= (1 << ADPS0) | (1 << ADPS1);

    /* Start the ADC */
    ADCSRA |= (1 << ADEN);
}

uint16_t readADC(){
    /* START CONVERSION - page 218
    *  Enable bit to start ADC
    *  This bit returns to 0 after finishing automatically, so we wait for it to finish.
    */
    ADCSRA |= (1 << ADSC);
    while((ADCSRA & (1 << ADSC)));
    
    /* READ CONVERSION - page 219
    *  Read 8 low bits from the ADCL register
    *  Read 8 high bits from the ADCH register
    */
    uint16_t result = ADCL + (ADCH << 8);
    return result;
}

int main(int argc, char **argv){

    //configure analog pin reading
    configureADC();

    //configure the serial port
    configureSerial(9600);

    //configure printf
    FILE f_out;
    configurePrintf(&f_out);

    int i = 0;
    for(;;){
        //send a serial message
        printf("%d\n",readADC());

        //busy waiting
        _delay_ms(500);
    }

    return 0;
}