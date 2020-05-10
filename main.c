#include <mega8.h>
#include <delay.h>

eeprom unsigned int d;

interrupt [EXT_INT1] void ext_int1_isr(void)
  {
  //Timer1 Start
    TCCR1B=(0<<CS12) | (0<<CS11) | (1<<CS10);
    //Timer1= &HFFFF -d
    OCR1AH=(0xFF-d)>>8;
    OCR1AL=(0xFF-d)&0xff;
  }

interrupt [TIM1_COMPA] void timer1_compa_isr(void)
  {
  //Timer1 Stop
  TCCR1B=(0<<CS12) | (0<<CS11) | (0<<CS10);
  //Timer1 = 0;
  TCNT1H=0x00;
  TCNT1L=0x00;
  //Pulse
  PORTD.4=0;
  delay_us(10);
  PORTD.4=1;
  }

void main(void)
  {
   // Input/Output Ports initialization
  DDRD.6=DDRD.7=0;
  DDRD.4=1;
  // Timer/Counter 1 initialization
  TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
  TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
  OCR1AH=0xFF;
  OCR1AL=0xFF;
  // Timer(s)/Counter(s) Interrupt(s) initialization
  TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<TOIE0);
  // External Interrupt(s) initialization
  GICR|=(1<<INT1) | (0<<INT0);
  MCUCR=(1<<ISC11) | (1<<ISC10) | (0<<ISC01) | (0<<ISC00);
  GIFR=(1<<INTF1) | (0<<INTF0);
  // Global enable interrupts
  #asm("sei")
  while (1)
    {
    if(PIND.6==0)
      {
      if(d>=32767)d=32767;
      else d=d+100;
      }
    if(PIND.7==0)
      {
      if(d<=200)d=200;
      else d=d-100;
      }
    }
  }