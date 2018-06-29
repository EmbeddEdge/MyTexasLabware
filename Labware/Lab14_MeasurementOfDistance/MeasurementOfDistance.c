// MeasurementOfDistance.c
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to periodically initiate a software-
// triggered ADC conversion, convert the sample to a fixed-
// point decimal distance, and store the result in a mailbox.
// The foreground thread takes the result from the mailbox,
// converts the result to a String1, and prints it to the
// Nokia5110 LCD.  The display is optional.
// January 15, 2016

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// Slide pot pin 3 connected to +3.3V
// Slide pot pin 2 connected to PE2(Ain1) and PD3
// Slide pot pin 1 connected to ground


#include "ADC.h"
#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "TExaS.h"
#include "UART.h"

#define SP   0x20

enum OnboardLED
{
	Red		= 0x02,
	Blue 	= 0x04,
	Green 	= 0x08,
	Yellow 	= 0x0A,
	SkyBlue = 0x0C,
	White 	= 0x0E
};

void EnableInterrupts(void);  // Enable interrupts

unsigned char String1[10]; // null-terminated ASCII String1
unsigned long Distance;   // units 0.001 cm
unsigned long ADCdata;    // 12-bit 0 to 4095 sample
unsigned long Flag;       // 1 means valid Distance, 0 means Distance is empty

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

// **************HB_Trigger*********************
// Activate Heartbeat LEDs
// Input: LED pin
// Output: none
void HB_Trigger(enum OnboardLED color)
{
	GPIO_PORTF_DATA_R = color;  
}

// **************HB_Toggle*********************
// Toggle Heartbeat LEDs
// Input: LED pin
// Output: none
void HB_Toggle(enum OnboardLED color)
{
	GPIO_PORTF_DATA_R ^= color;  
}

//********Convert****************
// Convert a 12-bit binary ADC sample into a 32-bit unsigned
// fixed-point distance (resolution 0.001 cm).  Calibration
// data is gathered using known distances and reading the
// ADC value measured on PE1.  
// Overflow and dropout should be considered 
// Maps 0V-3.3V to 0.000cm-2.000cm or 0.000cm-6.200cm
// Input: sample  12-bit ADC sample
// Output: 32-bit distance (resolution 0.001cm)
unsigned long Convert(unsigned long sample)
{
	double A=1.514,//A=500
								B=1;
	//Distance = ((A*sample)>>10)+B;
	Distance = A*sample+B;
  return Distance; 
}

// Initialize SysTick interrupts to trigger at 40 Hz, 25 ms
void SysTick_Init(unsigned long period)
{
  NVIC_ST_CTRL_R = 0;            // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1; // set reload value to input parameter period
  NVIC_ST_CURRENT_R = 0;      	 // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
}
// executes every 25 ms, collects a sample, converts and stores in mailbox
void SysTick_Handler(void)
{ 
	HB_Toggle(Red);
	HB_Toggle(Red);
	ADCdata = ADC0_In();
	Distance = Convert(ADCdata);
	Flag = 1;
	HB_Toggle(Red);
}

//-----------------------ConvertDistance-----------------------
// Converts a 32-bit distance into an ASCII String1
// Input: 32-bit number to be converted (resolution 0.001cm)
// Output: store the conversion in global variable String1[10]
// Fixed format 1 digit, point, 3 digits, space, units, null termination
// Examples
//    4 to "0.004 cm"  
//   31 to "0.031 cm" 
//  102 to "0.102 cm" 
// 2210 to "2.210 cm"
//10000 to "*.*** cm"  any value larger than 9999 converted to "*.*** cm"
void ConvertDistance(unsigned long n){
// as part of Lab 11 implement this function
	unsigned long i,digitWhole,FirstDec,numFirstDec,SecondDec,numSecondDec,ThirdDec;
	if(n<=9999)
	{
		digitWhole = n/1000+'0';       // digit to the left of the decimal
		String1[0] = digitWhole;
		n=n%1000;											 //0-999
		String1[1] = '.';
	
		FirstDec = n/100+'0';
		numFirstDec = n/100 * 100;
		String1[2] = FirstDec;
		n=n-numFirstDec;
	
		SecondDec = n/10+'0';
		numSecondDec = n/10 * 10;
		String1[3] = SecondDec;
		n=n-numSecondDec;
	
		ThirdDec = n/1+'0';
		String1[4] = ThirdDec;
	
		String1[5] =  SP;
		String1[6] = 'c';
		String1[7] = 'm';
	}
	else
	{
		for(i=0;i<5;i++)
		{
			String1[i] = '*';
		}
		String1[1]	=	'.';
		String1[5] =  SP;
		String1[6] = 'c';
		String1[7] = 'm';
	}

}

// main1 is a simple main program allowing you to debug the ADC interface
int main1(void)
{ 
  TExaS_Init(ADC0_AIN1_PIN_PE2, SSI0_Real_Nokia5110_NoScope);
  ADC0_Init();    // initialize ADC0, channel 1, sequencer 3
  EnableInterrupts();
  while(1)
	{ 
    ADCdata = ADC0_In();
  }
}
// once the ADC is operational, you can use main2 to debug the convert to distance
int main2(void)
{ 
  TExaS_Init(ADC0_AIN1_PIN_PE2, SSI0_Real_Nokia5110_NoScope);
  ADC0_Init();    // initialize ADC0, channel 1, sequencer 3
  Nokia5110_Init();             // initialize Nokia5110 LCD
  EnableInterrupts();
  while(1)
	{ 
    ADCdata = ADC0_In();
    Nokia5110_SetCursor(0, 0);
    Distance = Convert(ADCdata);
    ConvertDistance(Distance); // from Lab 11
    Nokia5110_OutString(String1);    // output to Nokia5110 LCD (optional)
  }
}

//-----------------------Main Function-----------------------
// Initialize the ADC and the Nokia5110 Display (Or optionally the uart)
// Input: None
// Output: None
// once the ADC and convert to distance functions are operational,
// you should use this main to build the final solution with interrupts and mailbox
int main(void)
{ 
  volatile unsigned long delay;
  TExaS_Init(ADC0_AIN1_PIN_PE2, SSI0_Real_Nokia5110_Scope);
	ADC0_Init();				// initialize ADC0, channel 1, sequencer 3
	//UART_Init();				// initialize UART (optional)
	Nokia5110_Init();			// initialize Nokia5110 LCD (optional)
	SysTick_Init(2000000);		// initialize SysTick for 40 Hz interrupts and set reload value to 2000001 (2000000+1), which sets the period to 2000000*12.5ns=0.025ms 
	//HB_Init();    				// initialize profiling on PF1 (optional)
								//    wait for clock to stabilize
  EnableInterrupts();
// print a welcome message  (optional)
  while(1)
	{ 
		Flag = 0;
		// read mailbox
		if(Flag==1)
		{
			ConvertDistance(Distance);
			Nokia5110_SetCursor(0, 0); 
			Nokia5110_OutString(String1); 
			//Or
			//UART_OutString(String1); 
			//UART_OutChar('\n');
		}
// output to Nokia5110 LCD (optional)
  }
}
