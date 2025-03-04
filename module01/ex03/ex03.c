// • You are required to write a program that turns on and off LED D2 (PB1) at a
// frequency of 1Hz with a variable duty cycle going from 10 to 100%.
// • You must configure the registers of Timer1 to control the LED.
// • The duty cycle must be managed as follow:
// ◦ Pressing button SW1 increments the duty cycle by 10 %.
// ◦ Pressing button SW2 decrements the duty cycle by 10 %.

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

#define LED_2 (1 << PB1)
#define BUTT_SW1 (0 << PD2)
#define BUTT_SW1_ON (1 << PD2)
#define BUTT_SW2 (0 << PD4)
#define BUTT_SW2_ON (1 << PD4)
#define PRESCALER 256

void inc_dc(float * dc) {
	if (!dc || *dc >= 0.99f)
		return ;
	*dc += 0.1f;
}

void dec_dc(float *dc) {
	if (!dc || *dc <= 0.11f)
		return ;
	*dc -= 0.1f;
}

void set_compare_val(uint16_t top, float dc) {
	OCR1A = (top * (1 - dc)); // change compare value
	TCNT1 = 0; // reset clock
}

int main(void) {
	float dc = 0.1;
	uint16_t top = (F_CPU / PRESCALER / 2);

	DDRB |= LED_2;
	TCCR1B |= (1 << CS12); // 256 prescaler
	TCCR1A |= ((1 << COM1A1) | (1 << COM1A0)); // set when up-counting, clear when down-counting
	TCCR1A |= (1 << WGM11); // PWM phase correct
	TCCR1B |= (1 << WGM13); // PWM phase correct
	ICR1 = top;
	set_compare_val(top, dc);

	while (1) {
		if (!(PIND & BUTT_SW1_ON)) {
			inc_dc(&dc);
			set_compare_val(top, dc);
			_delay_ms(275);
		}
		if (!(PIND & BUTT_SW2_ON)) {
			dec_dc(&dc);
			set_compare_val(top, dc);
			_delay_ms(275);
		}
	}
}