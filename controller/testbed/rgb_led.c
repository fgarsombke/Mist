#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/lm3s1968.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "rgb_led.h"

void RGB_Init(void) {
  volatile unsigned long ulLoop;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
  ulLoop = SYSCTL_RCGC2_R;
  GPIO_PORTD_DIR_R |= 0x7;
  GPIO_PORTD_DEN_R |= 0x7;
}

void RGB_Set(color_t color) {
  GPIO_PORTD_DATA_R &= ~(0x7);
  GPIO_PORTD_DATA_R |= color;
}

void RGB_On(color_t color) {
  GPIO_PORTD_DATA_R |= color;
}
void RGB_Off(color_t color) {
  GPIO_PORTD_DATA_R &= ~(color);
}
  
