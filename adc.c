#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#define ADC 2
void stop();
void left_forward();
void right_forward();
void left_backward();
void right_backward();
void forward();
void backward();
void wait()
{
int i;
for(i=0;i<2;i++)
	_delay_ms(150);

}
void pwm(int duty)
{
	OCR0=duty;//output on PB3;
}
void initADC()
{
	ADMUX=(1<<REFS0)|(0<<ADLAR);// For Aref=AVcc;
	ADCSRA=(1<<ADEN)|(7<<ADPS0); //division factor 128
}
int ReadADC(int count)
{
//Select ADC Channel ch must be 0-7
	//ch=ch&0b00000111;
	ADMUX|=0x07;
	//Start Single conversion
	ADCSRA|=(1<<ADSC);
	//Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));
	int a=ADCW;
	//Clear ADIF by writing one to it
	ADCSRA|=(1<<ADIF);
	int b;
	int backcnt;

	if(a<((1023*2)/5)) {
		b=1;
		count++;
		if(count>15)		
		{ 	backcnt=4;
			stop();
			wait();
			backward();
			while(backcnt>=0)
			{
				wait();
				backcnt--;
			}
			backcnt=5;
			stop();
			wait();
			left_backward();
			while(backcnt>=0)
			{
				wait();
				backcnt--;
			}
			stop();
			wait();
			backcnt=2;
			backward();
			while(backcnt>=0)
			{
				wait();
				backcnt--;
			}
			stop();
			wait();
			count=100;		
		}
			
		
		else {
			forward();
			wait();
		}
	
	}
	else {
		forward();
		wait();
		b = 0;
	}

	return count;

}

int main()
{	
	int count=0;
	DDRC=0xff;
	PORTC=0x00;
	
	MCUCSR |= (1<<JTD);
	TCCR0=0x6A;
	TCNT0=0;
	DDRB=0x08;//0b00001000
	DDRD = 0xff;
	PORTD = 0xff;
	initADC();

	while(1)
	{	
		
		count = ReadADC(count);
		if(count==100)
			break;
	}
}

void forward()
{
	pwm(80);
	PORTD = 0b01010000;
}
void backward()
{
	pwm(80);
	PORTD = 0b10100000;
}
void left_forward()
{
	pwm(80);
	PORTD = 0b01000000;
}
void left_backward()
{
	pwm(80);
	PORTD = 0b10000000;
}
void right_forward()
{	
	pwm(80);
        PORTD = 0b00010000;
}
void right_backward()
{
	pwm(80);
        PORTD = 0b00100000;
}
void stop()
{
pwm(0);
PORTD = 0x00;
}
