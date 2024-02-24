#define F_CPU 1200000UL
#define LED1 PA2
#define LED2 PA4
#define BUTTON1 PA3 // PCINT3
#define BUTTON2 PA5
#define LEDOUT PA1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Обработчик прерывания PCINT0
ISR(PCINT0_vect)
{
   _delay_ms (50); // антидребезг (использовать задержки в прерываниях некошерно, но пока и так сойдёт)
  if ( (PINA & (1<<BUTTON1)) == 0) // если нажата одна из кнопок
  {
    PORTA ^= (1<<LED1); //переключаем состояние светодиода (вкл./выкл.)
    while ( (PINA & (1<<BUTTON1)) == 0 ) {} // ждём отпускания кнопки
  }
  if ( (PINA & (1<<BUTTON2)) == 0) // если нажата одна из кнопок
  {
    PORTA ^= (1<<LED2); //переключаем состояние светодиода (вкл./выкл.)
    while ( (PINA & (1<<BUTTON2)) == 0 ) {} // ждём отпускания кнопки
  } 
}

int main(void)
{

  // Пины кнопок
  DDRA &= ~(1<<BUTTON1); // входы
  PORTA |= (1<<BUTTON1); // подтянуты

	DDRA &= ~(1<<BUTTON2); // входы
  PORTA |= (1<<BUTTON2); // подтянуты
  // Пин светодиода
  DDRA |= (1<<LED1); // выход
  PORTA &= ~(1<<LED1); // выключен

  DDRA |= (1<<LED2); // выход
  PORTA &= ~(1<<LED2); // выключен

  DDRA |= (1<<LEDOUT); // выход
  PORTA &= ~(1<<LEDOUT); // выключен
  // Настройка прерываний
  GIMSK |= (1<<PCIE0); // Разрешаем внешние прерывания PCINT0.
  PCMSK0 |= (1<<BUTTON1); // Разрешаем по маске прерывания на ногак кнопок (PCINT3, PCINT4)
  PCMSK0 |= (1<<BUTTON2);
  sei(); // Разрешаем прерывания глобально: SREG |= (1<<SREG_I)
  while (1) 
  {
    if((PINA & (1<<LED1))||(PINA & (1<<LED2)))
      PORTA |= (1<<LEDOUT);
    else
      PORTA &= ~(1<<LEDOUT); 
  }
}