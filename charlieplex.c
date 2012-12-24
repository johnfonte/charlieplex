#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usb_debug_only.h"
#include "print.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

#define bool            uint8_t
#define true            1
#define false           0
#define NULL            0
#define NA              0

#define _DDRB           (uint8_t *const)&DDRB
#define _DDRC           (uint8_t *const)&DDRC
#define _DDRD           (uint8_t *const)&DDRD
#define _DDRE           (uint8_t *const)&DDRE
#define _DDRF           (uint8_t *const)&DDRF

#define _PINB           (uint8_t *const)&PINB
#define _PINC           (uint8_t *const)&PINC
#define _PIND           (uint8_t *const)&PIND
#define _PINE           (uint8_t *const)&PINE
#define _PINF           (uint8_t *const)&PINF

#define _PORTB          (uint8_t *const)&PORTB
#define _PORTC          (uint8_t *const)&PORTC
#define _PORTD          (uint8_t *const)&PORTD
#define _PORTE          (uint8_t *const)&PORTE
#define _PORTF          (uint8_t *const)&PORTF

#define _PIN0 0x01
#define _PIN1 0x02
#define _PIN2 0x04
#define _PIN3 0x08
#define _PIN4 0x10
#define _PIN5 0x20
#define _PIN6 0x40
#define _PIN7 0x80

#define PINS 3

/* Specifies the ports and pin numbers for charlieplexed LEDs */

uint8_t *const  led_ddr[PINS] = { _DDRF,  _DDRF,  _DDRF };
uint8_t *const led_port[PINS] = {_PORTF, _PORTF, _PORTF };
const uint8_t   led_bit[PINS] = { _PIN0,  _PIN1,  _PIN4 };

void init(void);

int main(void) {
  uint8_t i, j;

  init();

  for(;;) {
    _delay_ms(900);
    for(i=0; i<PINS; i++) {
      for(j=i+1; j<PINS; j++) {
		    //set pin i to out, pin j to in
        *led_ddr[i] |= led_bit[i];
        *led_port[i] |= led_bit[i]; //output high

        *led_ddr[j] |= led_bit[j];
        *led_port[j] &= ~led_bit[j]; //output low
        _delay_ms(50);
    		

        //set pin j to out, pin i to in
        *led_ddr[i] |= led_bit[i];
        *led_port[i] &= ~led_bit[i]; //output low

        *led_ddr[j] |= led_bit[j];
        *led_port[j] |= led_bit[j]; //output high
        _delay_ms(50);

    
    		//disable pins
        *led_ddr[i] &= ~led_bit[i];
        *led_port[i] &= ~led_bit[i]; //input

        *led_ddr[j] &= ~led_bit[j];
        *led_port[j] &= ~led_bit[j]; //input
      }
    }
  }
  
}

void init(void) {
  CLKPR = 0x80; CLKPR = 0;
  usb_init();
  while(!usb_configured());
  _delay_ms(1000);
  CPU_PRESCALE(0);

  //initialize to input
  uint8_t i;
  for(i=0; i<PINS; i++) {
    *led_ddr[i] &= ~led_bit[i];
    *led_port[i] &= ~led_bit[i];
  }
}
