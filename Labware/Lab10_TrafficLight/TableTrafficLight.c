// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
//#include "PLL.h"
#include "SysTick.h"

#define ROADLIGHTS        (*((volatile unsigned long *)0x400053FC))
#define PEDESTRIANLIGHTS  (*((volatile unsigned long *)0x400253FC))
#define ROADSENSORS       (*((volatile unsigned long *)0x400043FC))


// ***** 2. Global Declarations Section *****
//Linked Data Structure
struct State
{
	unsigned long trafficOut;
	unsigned long pedestrianOut;
	unsigned long Wait;
	unsigned long Next[8];
};
typedef const struct State Stype;

//States
#define GoWest    0
#define WaitWest  1
#define GoSouth	  2
#define WaitSouth 3
#define StopAllW  4
#define StopAllS  5
#define StopAllP  6
#define Walk  		7
#define HurryOff1 8
#define HurryR1   9
#define HurryOff2 10
#define HurryR2   11
#define HurryOff3 12
#define HurryR3   13
#define HurryOff4 14

Stype FSM[15]=
{
	{0x0C,0x02,60,{GoWest,GoWest,WaitWest,WaitWest,WaitWest,WaitWest,WaitWest,WaitWest}}, 										//1.  Initial State: GoWest
	{0x14,0x02,50,{StopAllW,StopAllW,StopAllW,StopAllW,StopAllW,StopAllW,StopAllW,StopAllW}}, 								//2.  WaitWest
	{0x21,0x02,60,{GoSouth,WaitSouth,GoSouth,WaitSouth,WaitSouth,WaitSouth,WaitSouth,WaitSouth}}, 						//3.  GoSouth
	{0x22,0x02,50,{StopAllS,StopAllS,StopAllS,StopAllS,StopAllS,StopAllS,StopAllS,StopAllS}}, 								//4.  WaitSouth
	{0x24,0x02,50,{GoSouth,GoWest,GoSouth,GoSouth,Walk,Walk,GoSouth,GoSouth}}, 															  //5.  StopAllW
	{0x24,0x02,50,{Walk,GoWest,GoSouth,GoWest,Walk,GoWest,Walk,Walk}}, 																		  	//6.  StopAllS
	{0x24,0x02,50,{GoWest,GoWest,GoSouth,GoWest,Walk,GoWest,GoSouth,GoWest}}, 																//7.  StopAllP
	{0x24,0x08,60,{HurryOff1,HurryOff1,HurryOff1,HurryOff1,Walk,HurryOff1,HurryOff1,HurryOff1}}, 				  		//8.  Walk
	{0x24,0x00,40,{HurryR1,HurryR1,HurryR1,HurryR1,HurryR1,HurryR1,HurryR1,HurryR1}}, 												//9.  HurryOff1
	{0x24,0x02,40,{HurryOff2,HurryOff2,HurryOff2,HurryOff2,HurryOff2,HurryOff2,HurryOff2,HurryOff2}}, 				//10. HurryR1
	{0x24,0x00,40,{HurryR2,HurryR2,HurryR2,HurryR2,HurryR2,HurryR2,HurryR2,HurryR2}}, 												//11. HurryOff2
	{0x24,0x02,40,{HurryOff3,HurryOff3,HurryOff3,HurryOff3,HurryOff3,HurryOff3,HurryOff3,HurryOff3}}, 				//12. HurryR2
	{0x24,0x00,40,{HurryR3,HurryR3,HurryR3,HurryR3,HurryR3,HurryR3,HurryR3,HurryR3}}, 												//13. HurryOff3
	{0x24,0x02,40,{HurryOff4,HurryOff4,HurryOff4,HurryOff4,HurryOff4,HurryOff4,HurryOff4,HurryOff4}}, 				//14. HurryR3
	{0x24,0x00,40,{StopAllP,StopAllP,StopAllP,StopAllP,StopAllP,StopAllP,StopAllP,StopAllP}},           			//15. HurryOff4
};

unsigned long cState;			//Index to the state
unsigned long Input;

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortB_Init(void);				// Configure and enable PortB for Traffic Light output
void PortF_Init(void);				// Configure and enable PortF for Pedestrian light output 
void PortA_Init(void);				// Configure and enable PortA for Cars and pedestrians sensor input

// ***** 3. Subroutines Section *****

int main(void)
{ 
  TExaS_Init(SW_PIN_PA432, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
	SysTick_Init();   // Program 10.2
	PortB_Init();
	PortF_Init();
	PortA_Init();
	
	cState = 0;
  
  EnableInterrupts();
  while(1)
	{
   ROADLIGHTS = FSM[cState].trafficOut;  
	 PEDESTRIANLIGHTS = FSM[cState].pedestrianOut;
	 SysTick_Wait10ms(FSM[cState].Wait);
	 Input = ROADSENSORS>>2;
	 cState = FSM[cState].Next[Input]; 
  }
}

void PortB_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x02;    			 //1. Activate clock for PortB
	delay = SYSCTL_RCGC2_R;    			 // allow time for clock to stabilize
	GPIO_PORTB_DIR_R = 0x3F;	 			 //2. Bits PB0-5 set as output
	GPIO_PORTB_AFSEL_R = 0x00; 			 //3. No alternate functions
	GPIO_PORTB_DEN_R	=	0x3F;	 			 //4. Set digital enable on the pins 
	GPIO_PORTB_PCTL_R = 0x00000000;  //5. configure PB7-0 as GPIO
	GPIO_PORTB_AMSEL_R &= ~0xFF;      //6. Disable analog function
	
}

void PortF_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x20;    			  //1. Activate clock for PortF
	delay = SYSCTL_RCGC2_R;    			  // allow time for clock to stabilize
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   //1a. unlock GPIO Port F
	GPIO_PORTF_CR_R = 0x0A;           // allow changes to PF3 & 1
	// only PF0 needs to be unlocked, other bits can't be locked
	GPIO_PORTF_DIR_R = 0x0A;	 			  //2. Bits PF3 & 1 set as output
	GPIO_PORTF_AFSEL_R = 0x00; 			  //3. No alternate functions
	GPIO_PORTF_DEN_R	=	0x0A;	 			  //4. Set digital enable on the pins 
	GPIO_PORTF_PCTL_R = 0x00000000;   //5. configure PF7-0 as GPIO
	GPIO_PORTF_AMSEL_R &= ~0xFF;      //6. Disable analog function
	
	// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06

}

void PortA_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x01;    			  //1. Activate clock for PortF
	delay = SYSCTL_RCGC2_R;						//Allow time for the clock to stabilize
	GPIO_PORTA_DIR_R = 0x00;					//2. Bits PA2-4 as inputs
	GPIO_PORTA_AFSEL_R = 0x00;				//3. No alternate functions
	GPIO_PORTA_DEN_R = 0x1C;					//4. Enable Digital on PA2-4
	GPIO_PORTA_PCTL_R = 0x00000000;		//5. Configure PA2-4 as GPIO
	GPIO_PORTA_AMSEL_R &= ~0xFF;			//6. Disable analog function
	//GPIO_PORTA_PDR_R = 0x1C;					//7. Activate Pull-down resistors on PA2-4
}




















