/*
 * This won't make a lot of sense unless you know how avr pwm works.
 * Take it on faith
 */
#include <avr/io.h>
#include <util/delay.h>

int main() {
    /* Configure PWM
     */

    // PB0 is OC0A, so set it as output
    DDRB |= (1 << PB0);

    // Configure clear on rising edge, set on falling edge
    TCCR0A = (1 << COM0A1) | (0 << COM0A0);
    // Configure phase correct PWM using OCR0A
    TCCR0A = (1 << WGM02) | TCCR0A;
    TCCR0B = (0 << WGM01) | (1 << WGM00);
    // Configure CLK/64 prescaler
    TCCR0B = (1 << CS01) | (1 << CS00) | TCCR0B;

    // Default PWM output
    OCR0A = 0x00;

    
    /* Configure ADC
     */

    // Voltage reference defaults to VCC, so no need to change

    // Change ADC to left adjust result
    ADMUX = (1 << ADLAR);

    // Again default is nice, it uses PB5 (ADC0) as a single-ended input
    
    // Disable digital input on ADC0-ADC
    DIDR0 = 0b00111100; 

    // Enable ADC
    ADCSRA = (1 << ADEN);


    /* Loop
     */
    while(1) {
        // Start ADC conversion
        ADCSRA = ADCSRA | (1 << ADSC);
        // Wait until conversion is finished
        while (ADCSRA & ADSC);

        // Set PWM value
        OCR0A = ADCH;

        // Update at 5Hz
        _delay_ms(200);

    }
}
