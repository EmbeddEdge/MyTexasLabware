// Sound.h
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014

enum OnboardLED
{
  Red 		= 0x01,
	Blue 		= 0x04,
	Green 	= 0x08,
	Yellow 	= 0x0A,
	SkyBlue = 0x0C,
	White 	= 0x0E
};

// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also initializes DAC
// Input: none
// Output: none
void Sound_Init(void);

// **************Sound_Tone*********************
// Change SysTick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long period);

// **************HB_Init*********************
// Configure PORTF for heartbeat LED's
// Input: none
// Output: none
void HB_Init(void);

// **************HB_Init*********************
// Configure PORTF for heartbeat LED's
// Input: none
// Output: none
void HB_Toggle(enum OnboardLED colour);

// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void);

// the 256 comes from the length of the sine wave table
// bus is 80 MHz
// Period =  80000000/32/Freq=5000000/Freq
#define C1   1195   // 2093 Hz
#define B1   1266   // 1975.5 Hz
#define BF1  1341   // 1864.7 Hz
#define A1   1421   // 1760 Hz
#define AF1  1505   // 1661.2 Hz
#define G1   1595   // 1568 Hz
#define GF1  1689   // 1480 Hz
#define F1   1790   // 1396.9 Hz
#define E1   1896   // 1318.5 Hz
#define EF1  2009   // 1244.5 Hz
#define D1   2129   // 1174.7 Hz
#define DF1  2255   // 1108.7 Hz
#define C    2389   // 1046.5 Hz
#define B    2531   // 987.8 Hz
#define BF   2682   // 932.3 Hz
#define A    2840   // 880 Hz
#define AF   3010   // 830.6 Hz
#define G    3189   // 784 Hz
#define GF  3379   // 740 Hz
#define F   3580   // 698.5 Hz
#define E   3792   // 659.3 Hz
#define EF  4018   // 622.3 Hz
#define D   4257   // 587.3 Hz
#define DF  4510   // 554.4 Hz
#define C0  4778   // 523.3 Hz
#define B0  5062   // 493.9 Hz
#define BF0 5363   // 466.2 Hz
#define A0  5682   // 440 Hz
#define AF0 6020   // 415.3 Hz
#define G0  6378   // 392 Hz
#define GF0 6757   // 370 Hz
#define F0  7159   // 349.2 Hz
#define E0  7585   // 329.6 Hz
#define EF0 8036   // 311.1 Hz
#define D0  8513   // 293.7 Hz
#define DF0 9020   // 277.2 Hz
#define C7  9556   // 261.6 Hz
#define B7  10124   // 246.9 Hz
#define BF7 10726   // 233.1 Hz
#define A7  11364   // 220 Hz
#define AF7 12040   // 207.7 Hz
#define G7  12756   // 196 Hz
#define GF7 13514   // 185 Hz
#define F7  14318   // 174.6 Hz
#define E7  15169   // 164.8 Hz
#define EF7 16071   // 155.6 Hz
#define D7  17026   // 146.8 Hz
#define DF7 18039   // 138.6 Hz
#define C6  19112   // 130.8 Hz
