// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"
#include "DAC.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);
int main(void)
{ // Real Lab13 
	// for the real board grader to work 
	// you must connect PD3 to your DAC output
	int n;
	Piano_Init();
	Sound_Init(); 	// initialize SysTick timer and DAC
  TExaS_Init(SW_PIN_PA5432, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz
// PortE used for piano keys, PortB used for DAC        
	//HB_Init();
  EnableInterrupts();  // enable after all initialization are done
  while(1)
	{               
// input from keys to select tone
		switch(Piano_In())
		{
			case 0x00:
				//HB_Toggle(Red);
				//DisableInterrupts();
				Sound_Off();
			break;
			case 0x01:
				//HB_Toggle(Green);
				//EnableInterrupts();
				Sound_Tone(C0);       //523.3Hz
				//Sound_Tone(C);
			break;
			case 0x02:
				//HB_Toggle(Yellow);
				//EnableInterrupts();
				//Sound_Tone(A);				
				Sound_Tone(D);				//587.3Hz
			break;
			case 0x04:
				//HB_Toggle(SkyBlue);
				//EnableInterrupts();
				//Sound_Tone(F);				
				Sound_Tone(E);				//659.3Hz
			break;
			case 0x08:
				//HB_Toggle(White);
				//EnableInterrupts();
				Sound_Tone(G);				//784Hz
			break;
			default:
				//DisableInterrupts();
				Sound_Off();
			break;	
		}
  }
            
}

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


