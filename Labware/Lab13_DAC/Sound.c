// Sound.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// This routine calls the 4-bit DAC

#include "Sound.h"
#include "DAC.h"
#include "..//tm4c123gh6pm.h"

unsigned char Index;
unsigned int SoundFlag = 0;

// 4-bit 32-element sine wave
const unsigned char SineWave[32] = {8,9,11,12,13,14,14,15,15,15,14,14,13,12,11,9,8,7,5,4,3,2,2,1,1,1,2,2,3,4,5,7};
const unsigned char GuitarWave[32] = {5,5,4,1,1,3,8,11,11,9,4,2,5,11,15,13,9,7,5,5,6,8,8,7,4,3,3,3,3,4,5,5};
	
// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void){
  Index = 0;
  NVIC_ST_CTRL_R = 0;            // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF; // set reload value to max as default, it will be recognfigured from inputs
  NVIC_ST_CURRENT_R = 0;      	 // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
	DAC_Init();          // Port B is DAC
}

// **************HB_Init*********************
// Initialize Heartbeat Pins
// Input: none
// Output: none
void HB_Init(void)
{
	volatile unsigned long  delay;
	SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}

// **************HB_Toggle*********************
// Toggle Heartbeat LEDs
// Input: LED pin
// Output: none
void HB_Toggle(enum OnboardLED color)
{
	GPIO_PORTF_DATA_R = color;  
}

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long period)
{
	// this routine sets the RELOAD and starts SysTick
	SoundFlag = 1;
	NVIC_ST_RELOAD_R = period + 1;
}


// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void){
 // this routine stops the sound output
	SoundFlag = 0;
	NVIC_ST_RELOAD_R = 0x00FFFFFF;
	DAC_Out(0);
}


// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
	//HB_Toggle(Blue);
	//GPIO_PORTF_DATA_R ^= 0x04;  //Heart
	if(SoundFlag==1)
	{
		DAC_Out(SineWave[Index]);
		Index = (Index+1)&0xFF;
		if(Index>31)
		{
			Index = 0;
		}
	}
}
