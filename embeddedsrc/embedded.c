#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char **argv){

    //set port 5 to be output
    DDRB = DDRB | (1 << DDB5);

    for(;;){
        //turn port 5 on
        PORTB = PORTB | (1 << PORTB5);
        _delay_ms(1000);

        //turn port 5 off
        PORTB = PORTB & ~(1 << PORTB5);
        _delay_ms(1000);
    }

    
    return 0;
}