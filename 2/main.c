#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void led(void)
{
  if((PIN_L1 & (1<<LED1_PIN))||(PIN_L2 & (1<<LED2_PIN)))
    PORT_LEDOUT |= (1<<LEDOUT_PIN);
  else
    PORT_LEDOUT &= ~(1<<LEDOUT_PIN); 
}

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
ISR(PCINT0_vect)
{
  _delay_ms (50); // антидребезг (использовать задержки в прерываниях некошерно, но пока и так сойдёт)
  if (((PIN_B1 & (1<<B1_PIN)) == 0)&&((PIN_SWITCH & (1<<SWITCH_PIN)) == 0)) // если нажата одна из кнопок
  {
    PORT_L1 ^= (1<<LED1_PIN); //переключаем состояние светодиода (вкл./выкл.)
    led();
    while ( (PIN_B1 & (1<<B1_PIN)) == 0 ) {} // ждём отпускания кнопки
  }
  if (((PIN_B2 & (1<<B2_PIN)) == 0)&&((PIN_SWITCH & (1<<SWITCH_PIN)) == 0)) // если нажата одна из кнопок
  {
    PORT_L2 ^= (1<<LED2_PIN); //переключаем состояние светодиода (вкл./выкл.)
    led();
    while ( (PIN_B2 & (1<<B2_PIN)) == 0 ) {} // ждём отпускания кнопки
  } 
}
#else
// Обработчик прерывания PCINT0
ISR(PCINT_VEC0)
{
  _delay_ms (50); // антидребезг (использовать задержки в прерываниях некошерно, но пока и так сойдёт)
  if (((PIN_B1 & (1<<B1_PIN)) == 0)&&((PIN_SWITCH & (1<<SWITCH_PIN)) == 0)) // если нажата одна из кнопок
  {
    PORT_L1 ^= (1<<LED1_PIN); //переключаем состояние светодиода (вкл./выкл.)
    led();
    while ( (PIN_B1 & (1<<B1_PIN)) == 0 ) {} // ждём отпускания кнопки
  }
  if (((PIN_B2 & (1<<B2_PIN)) == 0)&&((PIN_SWITCH & (1<<SWITCH_PIN)) == 0)) // если нажата одна из кнопок
  {
    PORT_L2 ^= (1<<LED2_PIN); //переключаем состояние светодиода (вкл./выкл.)
    led();
    while ( (PIN_B2 & (1<<B2_PIN)) == 0 ) {} // ждём отпускания кнопки
  } 
}
#endif

int main(void)
{

  // Пины кнопок
  DDR_B1 &= ~(1<<B1_PIN); // входы
  PORT_B1 |= (1<<B1_PIN); // подтянуты

	DDR_B2 &= ~(1<<B2_PIN); // входы
  PORT_B2 |= (1<<B2_PIN); // подтянуты

  DDR_SWITCH &= ~(1<<SWITCH_PIN);
  PORT_SWITCH |= (1<<SWITCH_PIN);

  // Пин светодиода
  DDR_L1 |= (1<<LED1_PIN); // выход
  PORT_L1 &= ~(1<<LED1_PIN); // выключен

  DDR_L2 |= (1<<LED2_PIN); // выход
  PORT_L2 &= ~(1<<LED2_PIN); // выключен

  DDR_LEDOUT |= (1<<LEDOUT_PIN); // выход
  PORT_LEDOUT &= ~(1<<LEDOUT_PIN); // выключен

  // Настройка прерываний
  GIMSK |= (1<<PCIE_B1); // Разрешаем внешние прерывания PCINT0.
  GIMSK |= (1<<PCIE_B2); // Разрешаем внешние прерывания PCINT0
  PCMSK_B1 |= (1<<B1_PIN); // Разрешаем по маске прерывания на ногак кнопок (PCINT3, PCINT4)
  PCMSK_B2 |= (1<<B2_PIN);
  sei(); // Разрешаем прерывания глобально: SREG |= (1<<SREG_I)
  while (1) 
  {
    if((PIN_SWITCH & (1<<SWITCH_PIN)) == 1)
    {
      PORT_L1 &= ~(1<<LED1_PIN);
      PORT_L2 &= ~(1<<LED2_PIN);
      PORT_LEDOUT &= ~(1<<LEDOUT_PIN);   
    }
  }
}
