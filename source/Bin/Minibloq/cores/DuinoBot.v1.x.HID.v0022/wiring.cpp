/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id$
*/

#include "wiring_private.h"

//##RG.Labs.20110305 begin
extern "C" {
	#include "wd.h"
}
#include "VirtualSerial.h"
#include "HID.h"
//##RG.Labs.20110305 end

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

SIGNAL(TIMER0_OVF_vect)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}

unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer0_millis;
	SREG = oldSREG;

	return m;
}

unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer0_overflow_count;
#if defined(TCNT0)
	t = TCNT0;
#elif defined(TCNT0L)
	t = TCNT0L;
#else
	#error TIMER 0 not defined
#endif

  
#ifdef TIFR0
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
		m++;
#else
	if ((TIFR & _BV(TOV0)) && (t < 255))
		m++;
#endif

	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

void delay(unsigned long ms)
{
	uint16_t start = (uint16_t)micros();

	while (ms > 0) {
		if (((uint16_t)micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}

/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */
void delayMicroseconds(unsigned int us)
{
	// calling avrlib's delay_us() function with low values (e.g. 1 or
	// 2 microseconds) gives delays longer than desired.
	//delay_us(us);

#if F_CPU >= 16000000L
	// for the 16 MHz clock on most Arduino boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call yields a delay of approximately 1 1/8 us.
	if (--us == 0)
		return;

	// the following loop takes a quarter of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2;

	// account for the time taken in the preceeding commands.
	us -= 2;
#else
	// for the 8 MHz internal clock on the ATmega168

	// for a one- or two-microsecond delay, simply return.  the overhead of
	// the function calls takes more than two microseconds.  can't just
	// subtract two, since us is unsigned; we'd overflow.
	if (--us == 0)
		return;
	if (--us == 0)
		return;

	// the following loop takes half of a microsecond (4 cycles)
	// per iteration, so execute it twice for each microsecond of
	// delay requested.
	us <<= 1;
    
	// partially compensate for the time taken by the preceeding commands.
	// we can't subtract any more than this or we'd overflow w/ small delays.
	us--;
#endif

	// busy wait
	__asm__ __volatile__ (
		"1: sbiw %0,1" "\n\t" // 2 cycles
		"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
}

void init()
{

//##RG.Labs.20110305 begin
// Custom init code for the DuinoBot V1.XX

#if defined(__AVR_ATmega32U4__)
	
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

/*
	// TODO: Hay que chequear si est conectado el cable o no!, sino no debera inicializar nada
	if ((GPIOR0 == 0xDC) && (GPIOR1 == 0xA6)){	
		// The reset occurs by a softreset from the bootloader after downloading the new firmware

		// The global state variables of the usb hardware are hardcoded to fake a connection
		USB_IsInitialized = true;		
		USB_DeviceState =4;
		USB_ConfigurationNumber = 1;
			
	}else if ((GPIOR0 == 0xDC) && (GPIOR1 == 0xA7)){
		// The reset occurs by a softreset from the user application, mostly caused by de rising edge of the DTR signal

		// The global state variables of the usb hardware are hardcoded to fake a connection	
		USB_IsInitialized = true;
		USB_DeviceState = 4;
		USB_ConfigurationNumber = 1;		
	
	}else{
		// In this case, the applicatin was programmed standalone without a bootloader or with a different one so the usb hardware must be initialized
		USB_Init();	
	}
	*/
	USB_Init();	

	sei();

	// BECAREFULL
	// Timer/Counter0, 1, and 3 share the same prescaler module, but the Timer/Counters can have different prescaler settings.
	
	//=====================================================================
	// Timer 0 Delay functions y OC0A como salida de PWM en pin 9
	//=====================================================================
		
	// on the ATmega168, timer 0 is also used for fast hardware pwm
	// (using phase-correct PWM would mean that timer 0 overflowed half as often
	// resulting in different millis() behavior on the ATmega8 and ATmega168)	
	sbi(TCCR0A, WGM01);
	sbi(TCCR0A, WGM00);

	// set timer 0 prescale factor to 64	
	cbi(TCCR0B, CS02);
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);

	// enable timer 0 overflow interrupt
	sbi(TIMSK0, TOIE0);		
	
	//----Configuracion para OC0A-----
	// El WGM0X ya esta definido por que lo usa arduino para el timer
	//sbi(TCCR0A,COM0A1);
	cbi(TCCR0A,COM0A0);
	cbi(TCCR0B,WGM02);
	
		
	//=====================================================================
	//Timer 3 PIN 6 only OC3A
	//=====================================================================

	// se libera el timer3 para compatibilizar el core con la RGLib
	// (se pierde la funcion PWM del PIN6)
	
	//OC4D pin connected
	//nOC4D pin Disconnected
	//sbi(TCCR3A,COM3A1);
	//cbi(TCCR3A,COM3A0);
	
	//cbi(TCCR3B, WGM33);
	//cbi(TCCR3B, WGM32);
	//cbi(TCCR3A, WGM31);
	//sbi(TCCR3A, WGM30);	

	//En ppo esta apagado el pwm
	//cbi(TCCR3B,CS43);
	//cbi(TCCR3B,CS42);
	//cbi(TCCR3B,CS41);
	//sbi(TCCR3B,CS40);	
	
	//=====================================================================
	//Timer 4 Motors PWM (A & B) y GP TIMER D
	//=====================================================================
		
	//OC4A pin connected
	//nOC4A pin Disconnected
	//sbi(TCCR4A,COM4A1);
	cbi(TCCR4A,COM4A0);
	
	//OC4B pin connected
	//nOC4B pin Disconnected
	//sbi(TCCR4A,COM4B1); 
	cbi(TCCR4A,COM4B0);
	
	//Enable PWM en A y B
	//Phase and Frequency Correct PWM
	sbi(TCCR4A,PWM4A);
	sbi(TCCR4A,PWM4B);
	
	//Prescaler --> Freq sea 31.35Khz.
	
	//En ppo esta apagado el pwm
	cbi(TCCR4B,CS43);
	cbi(TCCR4B,CS42);
	cbi(TCCR4B,CS41);
	sbi(TCCR4B,CS40);	
		
	//--------------------------------------------------------------------------------------
	// Timer 4D para el pin 5
	
	//OC4D pin connected
	//nOC4D pin Disconnected
	//sbi(TCCR4D,COM4D1);
	cbi(TCCR4D,COM4D0);
	
	sbi(TCCR4C,PWM4D);
		
	//Phase and Frequency Correct PWM
	cbi(TCCR4D,WGM41);
	sbi(TCCR4D,WGM40);
	
	//=====================================================================
	// A/D
	//=====================================================================

	// set a2d prescale factor to 128
	// 16 MHz / 128 = 125 KHz, inside the desired 50-200 KHz range.
	// XXX: this will not work properly for other clock speeds, and
	// this code should use F_CPU to determine the prescale factor.
	sbi(ADCSRA, ADPS2);
	sbi(ADCSRA, ADPS1);
	sbi(ADCSRA, ADPS0);

	// enable a2d conversions
	sbi(ADCSRA, ADEN);

	
	//=====================================================================
	//  USART
	//=====================================================================
	// We dont need to do anything
		
		
	//=====================================================================
	//  Watchdog de CDC
	//=====================================================================	
	
	// The watchdog timer is configured for interrupting every 15ms for the cdc and usb tasks.
	WD_SET(WD_IRQ,WDTO_15MS);
	
#else
//##RG.Labs.20110305 end


	// this needs to be called before setup() or some functions won't
	// work there
	sei();
	
	// on the ATmega168, timer 0 is also used for fast hardware pwm
	// (using phase-correct PWM would mean that timer 0 overflowed half as often
	// resulting in different millis() behavior on the ATmega8 and ATmega168)
#if defined(TCCR0A) && defined(WGM01)
	sbi(TCCR0A, WGM01);
	sbi(TCCR0A, WGM00);
#endif  

	// set timer 0 prescale factor to 64
#if defined(__AVR_ATmega128__)
	// CPU specific: different values for the ATmega128
	sbi(TCCR0, CS02);
#elif defined(TCCR0) && defined(CS01) && defined(CS00)
	// this combination is for the standard atmega8
	sbi(TCCR0, CS01);
	sbi(TCCR0, CS00);
#elif defined(TCCR0B) && defined(CS01) && defined(CS00)
	// this combination is for the standard 168/328/1280/2560
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);
#elif defined(TCCR0A) && defined(CS01) && defined(CS00)
	// this combination is for the __AVR_ATmega645__ series
	sbi(TCCR0A, CS01);
	sbi(TCCR0A, CS00);
#else
	#error Timer 0 prescale factor 64 not set correctly
#endif

	// enable timer 0 overflow interrupt
#if defined(TIMSK) && defined(TOIE0)
	sbi(TIMSK, TOIE0);
#elif defined(TIMSK0) && defined(TOIE0)
	sbi(TIMSK0, TOIE0);
#else
	#error	Timer 0 overflow interrupt not set correctly
#endif

	// timers 1 and 2 are used for phase-correct hardware pwm
	// this is better for motors as it ensures an even waveform
	// note, however, that fast pwm mode can achieve a frequency of up
	// 8 MHz (with a 16 MHz clock) at 50% duty cycle

	TCCR1B = 0;

	// set timer 1 prescale factor to 64
#if defined(TCCR1B) && defined(CS11) && defined(CS10)
	sbi(TCCR1B, CS11);
	sbi(TCCR1B, CS10);
#elif defined(TCCR1) && defined(CS11) && defined(CS10)
	sbi(TCCR1, CS11);
	sbi(TCCR1, CS10);
#endif
	// put timer 1 in 8-bit phase correct pwm mode
#if defined(TCCR1A) && defined(WGM10)
	sbi(TCCR1A, WGM10);
#elif defined(TCCR1)
	#warning this needs to be finished
#endif

	// set timer 2 prescale factor to 64
#if defined(TCCR2) && defined(CS22)
	sbi(TCCR2, CS22);
#elif defined(TCCR2B) && defined(CS22)
	sbi(TCCR2B, CS22);
#else
	#warning Timer 2 not finished (may not be present on this CPU)
#endif

	// configure timer 2 for phase correct pwm (8-bit)
#if defined(TCCR2) && defined(WGM20)
	sbi(TCCR2, WGM20);
#elif defined(TCCR2A) && defined(WGM20)
	sbi(TCCR2A, WGM20);
#else
	#warning Timer 2 not finished (may not be present on this CPU)
#endif

#if defined(TCCR3B) && defined(CS31) && defined(WGM30)
	sbi(TCCR3B, CS31);		// set timer 3 prescale factor to 64
	sbi(TCCR3B, CS30);
	sbi(TCCR3A, WGM30);		// put timer 3 in 8-bit phase correct pwm mode
#endif
	
#if defined(TCCR4B) && defined(CS41) && defined(WGM40)
	sbi(TCCR4B, CS41);		// set timer 4 prescale factor to 64
	sbi(TCCR4B, CS40);
	sbi(TCCR4A, WGM40);		// put timer 4 in 8-bit phase correct pwm mode
#endif

#if defined(TCCR5B) && defined(CS51) && defined(WGM50)
	sbi(TCCR5B, CS51);		// set timer 5 prescale factor to 64
	sbi(TCCR5B, CS50);
	sbi(TCCR5A, WGM50);		// put timer 5 in 8-bit phase correct pwm mode
#endif

#if defined(ADCSRA)
	// set a2d prescale factor to 128
	// 16 MHz / 128 = 125 KHz, inside the desired 50-200 KHz range.
	// XXX: this will not work properly for other 	 speeds, and
	// this code should use F_CPU to determine the prescale factor.
	sbi(ADCSRA, ADPS2);
	sbi(ADCSRA, ADPS1);
	sbi(ADCSRA, ADPS0);

	// enable a2d conversions
	sbi(ADCSRA, ADEN);
#endif

	// the bootloader connects pins 0 and 1 to the USART; disconnect them
	// here so they can be used as normal digital i/o; they will be
	// reconnected in Serial.begin()
#if defined(UCSRB)
	UCSRB = 0;
#elif defined(UCSR0B)
	UCSR0B = 0;
#endif

//##RG.Labs.20110305 begin
#endif
//##RG.Labs.20110305 end
}

