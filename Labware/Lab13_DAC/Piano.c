// Piano.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// There are four keys in the piano
// Daniel Valvano
// December 29, 2014

// Port E bits 3-0 have 4 piano keys

#include "Piano.h"
#include "..//tm4c123gh6pm.h"


// **************Piano_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void Piano_Init(void){ 
  volatile unsigned long  delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;     // 1) activate clock for Port A
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  //GPIO_PORTA_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port A
  //GPIO_PORTA_CR_R = 0x3C;           // allow changes to PA5-2
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTA_AMSEL_R = 0x00;        // 3) disable analog on PA
  GPIO_PORTA_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PA5-2
  GPIO_PORTA_DIR_R = 0x00;          // 5) PA5-2 in
  GPIO_PORTA_AFSEL_R = 0x00;        // 6) disable alt funct on PA7-0
  //GPIO_PORTA_PUR_R = 0x3C;          // enable pull-up on PA5-2
  GPIO_PORTA_DEN_R = 0x3C;          // 7) enable digital I/O on PF4-0
}
// **************Piano_In*********************
// Input from piano key inputs
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void){
  
  return (GPIO_PORTA_DATA_R&0x3C)>>2; // read the four keys
}
