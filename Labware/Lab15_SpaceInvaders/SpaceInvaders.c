// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the edX Lab 15
// In order for other students to play your game
// 1) You must leave the hardware configuration as defined
// 2) You must not add/remove any files from the project
// 3) You must add your code only this this C file
// I.e., if you wish to use code from sprite.c or sound.c, move that code in this file
// 4) It must compile with the 32k limit of the free Keil

// April 10, 2014
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "stdio.h"
#include "stdlib.h"

// *************************** Images ***************************
// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10
const unsigned char SmallEnemy30PointA[] = {
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x0F, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
 0xFF, 0x0F, 0xF0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// enemy ship that starts at the top of the screen (arms/mouth open)
// width=16 x height=10
const unsigned char SmallEnemy30PointB[] = {
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
 0x0F, 0x00, 0x00, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
 0xFF, 0x0F, 0xF0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// enemy ship that starts in the middle of the screen (arms together)
// width=16 x height=10
const unsigned char SmallEnemy20PointA[] = {
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
 0xF0, 0x0F, 0xF0, 0x0F, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// enemy ship that starts in the middle of the screen (arms apart)
// width=16 x height=10
const unsigned char SmallEnemy20PointB[] = {
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0,
 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
 0xF0, 0x0F, 0xF0, 0x0F, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// enemy ship that starts at the bottom of the screen (arms down)
// width=16 x height=10
const unsigned char SmallEnemy10PointA[] = {
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x0F, 0xF0, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0F,
 0xF0, 0xFF, 0xFF, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// enemy ship that starts at the bottom of the screen (arms up)
// width=16 x height=10
const unsigned char SmallEnemy10PointB[] = {
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
 0xF0, 0xFF, 0xFF, 0x0F, 0xFF, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// image of the player's ship
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=18 x height=8
const unsigned char PlayerShip0[] = {
 0x42, 0x4D, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x00,
 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0xAA, 0xAA, 0xAA, 0xAA,
 0xAA, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0xAA, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0xAA, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// small, fast bonus enemy that occasionally speeds across the top of the screen after enough enemies have been killed to make room for it
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=20 x height=7
const unsigned char SmallEnemyBonus0[] = {
 0x42, 0x4D, 0xCA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x0F, 0xF0, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0xFF, 0x0F, 0xF0,
 0xFF, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// small shield floating in space to cover the player's ship from enemy fire (undamaged)
// width=18 x height=5
const unsigned char Bunker0[] = {
 0x42, 0x4D, 0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0x00,
 0x00, 0x00, 0xAA, 0xAA, 0xA0, 0x00, 0x00, 0x00, 0x0A, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x0A, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
 0xAA, 0xAA, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0xFF};

// small shield floating in space to cover the player's ship from enemy fire (moderate generic damage)
// width=18 x height=5
const unsigned char Bunker1[] = {
 0x42, 0x4D, 0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0x00,
 0x00, 0x00, 0xAA, 0xAA, 0xA0, 0x00, 0x00, 0x00, 0x0A, 0xAA, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x0A, 0xAA, 0xAA, 0xAA, 0xAA, 0xA0, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x0A, 0xAA, 0x0A, 0xA0, 0xA0, 0xAA,
 0xAA, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0xA0, 0x00, 0x0A, 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x00, 0xFF};

// small shield floating in space to cover the player's ship from enemy fire (heavy generic damage)
// width=18 x height=5
const unsigned char Bunker2[] = {
 0x42, 0x4D, 0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0xAA, 0x00,
 0x00, 0x00, 0xAA, 0x0A, 0xA0, 0x00, 0x00, 0x00, 0x0A, 0xAA, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x0A, 0x0A, 0xA0, 0xAA, 0xA0, 0xA0, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x0A, 0x00, 0xA0, 0xA0,
 0xA0, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// blank space used to cover a bunker that is destroyed
// width=18 x height=5
const unsigned char Bunker3[] = {
 0x42, 0x4D, 0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// large explosion that can be used upon the demise of the player's ship (first frame)
// width=18 x height=8
const unsigned char BigExplosion0[] = {
 0x42, 0x4D, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x0A, 0x00, 0x09, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x90, 0x00, 0x90, 0xB0, 0x0A, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0xA0, 0x00, 0xE0, 0x00, 0x00,
 0x90, 0x90, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0xA0, 0x00, 0x00, 0xAE, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00,
 0x00, 0xE0, 0x0A, 0x0A, 0x00, 0x00, 0xB0, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x09, 0x00, 0x00, 0x00, 0x90, 0x00, 0x0A, 0x00, 0x00, 0x00, 0xFF};

// large explosion that can be used upon the demise of the player's ship (second frame)
// width=18 x height=8
const unsigned char BigExplosion1[] = {
 0x42, 0x4D, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x0E, 0x00, 0x09, 0x00, 0x09, 0x00, 0xB0, 0x00, 0xA0, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA9, 0x00, 0x00, 0x00, 0x00, 0x90,
 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x0A, 0x00, 0x90, 0x00, 0xB0, 0x00, 0x09, 0x00, 0x00, 0x00, 0xFF};

// small explosion best used for the demise of an enemy
// width=16 x height=10
const unsigned char SmallExplosion0[] = {
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0,
 0x0F, 0x00, 0x0F, 0x00, 0xF0, 0x00, 0x00, 0x0F, 0x00, 0xF0, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
 0xF0, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xF0, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// blank space following the small explosion for the demise of an enemy
// width=16 x height=10
const unsigned char SmallExplosion1[] = {
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// a missile in flight
// includes one blacked out row on the top, bottom, and right of the image to prevent smearing when moved 1 pixel down, up, or left
// width=4 x height=9
const unsigned char Missile0[] = {
 0x42, 0x4D, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x00,
 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// a missile in flight
// includes one blacked out row on the top, bottom, and left of the image to prevent smearing when moved 1 pixel down, up, or right
// width=4 x height=9
const unsigned char Missile1[] = {
 0x42, 0x4D, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x0F,
 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// blank space to cover a missile after it hits something
// width=4 x height=9
const unsigned char Missile2[] = {
 0x42, 0x4D, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// a laser burst in flight
// includes one blacked out row on the top and bottom of the image to prevent smearing when moved 1 pixel up or down
// width=2 x height=9
const unsigned char Laser0[] = {
 0x42, 0x4D, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0xBB, 0x00,
 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

// blank space to cover a laser after it hits something
// width=2 x height=9
const unsigned char Laser1[] = {
 0x42, 0x4D, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};

const unsigned char titlescreennew[] ={
 0x42, 0x4D, 0xB6, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x40, 0x08, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x0F, 0xFF, 0x7F, 0xF7,
 0x77, 0xFF, 0x77, 0x77, 0xFF, 0x70, 0x0F, 0xF7, 0x77, 0x7F, 0xF7, 0xFF, 0xF7, 0x77, 0x77, 0x7F, 0xFF, 0xFF, 0x00, 0xFF, 0x77, 0x7F, 0xF0, 0x0F, 0xFF, 0x70, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x7F, 0xFF, 0x7F, 0xF7, 0x0F, 0xFF, 0x77, 0x77, 0xFF, 0x70, 0x0F, 0xF7, 0x70, 0x7F, 0xF7, 0xFF, 0xFF, 0xFF, 0x77, 0x7F, 0xFF, 0xFF, 0x07, 0xFF,
 0x77, 0xFF, 0xF0, 0xFF, 0xFF, 0xF7, 0x70, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x7F, 0xFF, 0x7F, 0xF7, 0x7F, 0xFF, 0x77, 0x7F, 0xFF, 0x70, 0x07, 0xFF,
 0xFF, 0xFF, 0xF7, 0xFF, 0x7F, 0xFF, 0xF0, 0x7F, 0xF0, 0x0F, 0x07, 0xFF, 0x77, 0xFF, 0x07, 0xF7, 0x77, 0xFF, 0x70, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x07,
 0x7F, 0xFF, 0x7F, 0xF7, 0xFF, 0xFF, 0x70, 0xFF, 0xFF, 0xF0, 0x07, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0x07, 0x7F, 0xF7, 0x7F, 0xF0, 0x0F, 0x07, 0xFF, 0x77, 0xFF, 0x07, 0x77, 0x77, 0xFF, 0x70, 0x0F,
 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x07, 0xFF, 0xFF, 0x7F, 0x77, 0xFF, 0xFF, 0x70, 0xFF, 0x7F, 0xF7, 0x07, 0xFF, 0x00, 0xFF, 0xF0, 0xFF, 0x00, 0x7F, 0xF7, 0x7F,
 0xF0, 0x00, 0x0F, 0xFF, 0xFF, 0xF7, 0x07, 0x77, 0x7F, 0xFF, 0x70, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x07, 0xFF, 0xF7, 0x7F, 0x7F, 0xF7, 0xFF, 0x77, 0xFF,
 0x7F, 0xF7, 0x00, 0xFF, 0x00, 0xFF, 0xF0, 0xFF, 0x00, 0x7F, 0xF7, 0x7F, 0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0xFF, 0x07, 0x7F, 0xFF, 0xFF, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x0F, 0x00, 0x07, 0xFF, 0xF7, 0xFF, 0xFF, 0x77, 0xFF, 0x7F, 0xF7, 0x77, 0xF7, 0x00, 0xFF, 0x0F, 0xFF, 0x00, 0xFF, 0x00, 0x7F, 0xF7, 0x7F, 0xF0, 0x00, 0x07, 0xFF, 0x00, 0xFF, 0xF0, 0xFF,
 0xF7, 0x77, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x07, 0xFF, 0xF7, 0xFF, 0xFF, 0x7F, 0xFF, 0x7F, 0xFF, 0x77, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF,
 0x00, 0x7F, 0xF7, 0x7F, 0xF0, 0x0F, 0x00, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x77, 0x70, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x07, 0xFF, 0xF7, 0xFF, 0xF7,
 0x7F, 0xF7, 0x7F, 0xFF, 0x70, 0xFF, 0x70, 0x7F, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0x70, 0x00, 0xFF, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x07, 0xFF, 0xF7, 0xFF, 0xF7, 0x7F, 0xF7, 0xFF, 0xFF, 0x70, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x70, 0x7F, 0xFF, 0xFF, 0xF0, 0xFF,
 0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x07, 0xFF, 0xF7, 0xFF, 0xFF, 0x7F, 0xF7, 0xFF, 0xF7, 0x00, 0xFF, 0xF0, 0x0F,
 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0x70, 0x7F, 0xFF, 0xFF, 0x70, 0xFF, 0xFF, 0xF7, 0x07, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00,
 0xFF, 0xF0, 0xFF, 0xFF, 0x0F, 0xF7, 0xFF, 0xF7, 0x00, 0x0F, 0xF0, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0x70, 0x0F, 0xFF, 0xFF, 0x70, 0xFF, 0xFF, 0xF7, 0x07, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x0F, 0xFF, 0x00, 0xF7, 0xFF, 0x70, 0x00, 0x0F, 0xF0, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0x0F,
 0xFF, 0xF7, 0x00, 0xFF, 0xFF, 0x70, 0x00, 0x7F, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xFF, 0xFF, 0x70, 0x0F, 0xF7, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0x70, 0x0F, 0xF7, 0x00, 0x00, 0x7F, 0xF0, 0x00, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF,
 0xFF, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xF7, 0xFF, 0xF0, 0x0F, 0xF7, 0x00,
 0x00, 0x7F, 0xF7, 0x77, 0xFF, 0x70, 0xFF, 0xFF, 0x7F, 0xF7, 0xFF, 0xFF, 0xFF, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x77, 0x7F, 0xF7, 0x0F, 0xF7, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0x77, 0xFF, 0x77, 0x77, 0xF7, 0xFF, 0xF7, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x7F, 0xF7, 0x7F, 0xF7, 0x77, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0x77, 0xFF, 0x77,
 0x77, 0x77, 0xFF, 0xF7, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x7F, 0xFF,
 0xF7, 0x7F, 0xFF, 0xF7, 0x70, 0x07, 0xFF, 0x00, 0xFF, 0x77, 0xFF, 0x70, 0x07, 0x70, 0xFF, 0xF7, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0x77, 0x7F, 0xFF, 0xFF, 0x77, 0x07, 0xFF, 0x00, 0xF7, 0x07, 0xFF, 0x70, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x77, 0x7F, 0xF0, 0xFF, 0x77, 0x07, 0xFF, 0x0F,
 0xF7, 0x07, 0xFF, 0x70, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x07, 0xFF, 0xFF, 0x77, 0x77, 0x7F, 0xF0, 0x0F, 0xF7, 0x07, 0xFF, 0x0F, 0xF7, 0x07, 0xFF, 0x70, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x77, 0x77, 0x70, 0xFF, 0xF0, 0x0F, 0xF7, 0x00, 0x7F, 0xFF, 0xF7, 0x07, 0xFF, 0x70, 0x00, 0x00, 0xFF, 0xF7,
 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x77, 0x7F, 0xF0, 0xFF, 0xF0, 0x0F,
 0xF7, 0x00, 0x7F, 0xFF, 0xF7, 0x07, 0xFF, 0xF7, 0x00, 0x77, 0xFF, 0xF7, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xF7, 0x00, 0x7F, 0xFF, 0x70, 0x07, 0x7F, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xF7, 0x00, 0x7F, 0xFF, 0x70, 0x00, 0x7F, 0xFF,
 0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xFF,
 0xF0, 0xFF, 0xFF, 0xFF, 0x77, 0x00, 0x7F, 0xFF, 0x70, 0x00, 0x0F, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x77, 0xFF, 0xFF, 0x77, 0xFF, 0xFF, 0xFF, 0x70, 0x00, 0x7F, 0xFF, 0x70, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xF7, 0x77, 0xFF, 0xFF, 0xF7, 0x70, 0x00, 0x77, 0xF7,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x07, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,
 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
};
// *************************** Sounds ***************************
const unsigned char explosion[2000] = {
  120, 119, 119, 119, 120, 120, 129, 130, 133, 129, 125, 119, 119, 119, 125, 128, 135, 137, 133, 123, 
  109, 99, 91, 92, 101, 116, 135, 140, 143, 130, 123, 105, 96, 89, 92, 105, 115, 116, 120, 119, 
  130, 133, 139, 149, 163, 171, 174, 173, 161, 143, 133, 115, 99, 79, 72, 75, 79, 82, 87, 103, 
  125, 143, 139, 137, 137, 149, 163, 173, 168, 153, 133, 111, 105, 106, 109, 119, 137, 149, 140, 133, 
  133, 143, 154, 168, 183, 187, 188, 184, 173, 163, 143, 113, 75, 39, 5, 0, 0, 3, 9, 19, 
  46, 82, 105, 120, 135, 139, 144, 173, 195, 208, 211, 202, 191, 181, 181, 202, 236, 255, 255, 255, 
  255, 255, 240, 212, 192, 171, 130, 85, 47, 36, 41, 37, 33, 43, 57, 51, 33, 3, 0, 0, 
  0, 0, 5, 9, 24, 67, 120, 157, 177, 201, 222, 239, 250, 246, 236, 216, 192, 159, 101, 41, 
  0, 0, 0, 0, 0, 0, 58, 103, 129, 161, 192, 218, 239, 242, 221, 184, 163, 157, 157, 159, 
  163, 161, 129, 81, 37, 17, 7, 19, 41, 61, 89, 139, 178, 195, 205, 202, 184, 161, 153, 157, 
  161, 181, 187, 177, 164, 149, 161, 197, 246, 255, 255, 255, 255, 255, 252, 226, 173, 105, 39, 0, 
  0, 0, 19, 71, 101, 133, 168, 183, 174, 163, 139, 113, 99, 77, 47, 31, 36, 41, 39, 43, 
  77, 125, 154, 149, 128, 95, 65, 57, 79, 99, 130, 161, 173, 192, 221, 246, 255, 235, 181, 115, 
  48, 0, 0, 0, 19, 58, 115, 171, 216, 216, 177, 119, 67, 43, 39, 31, 24, 17, 12, 15, 
  27, 79, 149, 226, 255, 255, 255, 255, 255, 255, 255, 240, 207, 157, 77, 3, 0, 0, 0, 0, 
  0, 0, 41, 72, 79, 67, 67, 95, 115, 125, 128, 120, 125, 147, 188, 222, 255, 255, 255, 255, 
  255, 255, 249, 255, 255, 255, 255, 255, 202, 140, 85, 29, 0, 0, 0, 0, 0, 46, 113, 129, 
  113, 79, 41, 17, 27, 61, 79, 92, 103, 103, 106, 115, 125, 154, 212, 255, 255, 255, 255, 255, 
  255, 222, 163, 115, 85, 96, 128, 163, 192, 207, 195, 143, 72, 0, 0, 0, 0, 0, 0, 0, 
  0, 53, 135, 201, 255, 255, 255, 255, 255, 222, 183, 144, 106, 63, 0, 0, 0, 0, 0, 0, 
  0, 29, 133, 192, 212, 207, 216, 245, 255, 255, 255, 249, 211, 205, 221, 250, 255, 255, 255, 255, 
  212, 140, 106, 105, 135, 167, 195, 218, 216, 192, 164, 135, 115, 105, 96, 92, 95, 103, 109, 119, 
  115, 85, 47, 15, 15, 47, 87, 115, 135, 149, 159, 188, 232, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 183, 106, 55, 17, 27, 58, 87, 129, 174, 218, 239, 215, 161, 95, 31, 0, 0, 0, 
  2, 17, 39, 87, 129, 147, 157, 168, 183, 188, 168, 115, 65, 31, 29, 33, 29, 36, 53, 61, 
  65, 70, 85, 92, 82, 82, 75, 58, 41, 48, 58, 72, 103, 149, 192, 229, 239, 226, 201, 164, 
  144, 143, 147, 137, 119, 103, 87, 85, 115, 161, 221, 255, 255, 246, 236, 218, 208, 183, 150, 128, 
  111, 101, 92, 99, 116, 135, 144, 149, 157, 163, 164, 164, 167, 159, 154, 154, 163, 173, 183, 187, 
  187, 191, 187, 177, 167, 161, 150, 137, 119, 106, 101, 99, 99, 96, 103, 105, 99, 92, 79, 70, 
  70, 71, 75, 77, 77, 82, 101, 123, 137, 144, 154, 163, 174, 177, 173, 173, 161, 153, 137, 111, 
  82, 53, 29, 22, 29, 37, 58, 87, 106, 119, 133, 149, 163, 173, 174, 164, 147, 137, 135, 133, 
  135, 139, 135, 120, 95, 72, 65, 58, 65, 75, 85, 99, 119, 130, 140, 147, 143, 135, 137, 153, 
  168, 168, 171, 161, 139, 123, 106, 113, 135, 171, 207, 235, 255, 255, 255, 255, 255, 221, 154, 77, 
  17, 0, 3, 46, 79, 91, 103, 120, 128, 119, 113, 92, 70, 51, 31, 17, 7, 12, 27, 48, 
  81, 130, 187, 212, 202, 171, 139, 113, 106, 116, 119, 129, 140, 140, 153, 171, 191, 195, 173, 130, 
  92, 57, 23, 13, 22, 36, 58, 99, 143, 177, 183, 157, 119, 85, 71, 77, 89, 91, 79, 71, 
  71, 79, 119, 171, 216, 255, 255, 249, 236, 225, 211, 202, 191, 173, 143, 82, 19, 0, 0, 0, 
  0, 0, 0, 46, 72, 79, 67, 70, 95, 113, 123, 123, 120, 123, 144, 178, 205, 240, 255, 255, 
  255, 255, 246, 236, 246, 255, 255, 255, 252, 205, 150, 101, 43, 0, 0, 0, 0, 0, 48, 109, 
  123, 105, 71, 36, 15, 24, 58, 77, 95, 103, 101, 106, 113, 128, 154, 212, 255, 255, 255, 255, 
  255, 255, 222, 163, 115, 85, 96, 128, 163, 192, 207, 195, 143, 72, 0, 0, 0, 0, 0, 0, 
  0, 0, 53, 135, 201, 255, 255, 255, 255, 255, 222, 183, 144, 106, 63, 0, 0, 0, 0, 0, 
  0, 0, 29, 133, 192, 212, 207, 216, 245, 255, 255, 255, 249, 211, 205, 221, 250, 255, 255, 255, 
  255, 212, 140, 106, 105, 135, 167, 195, 218, 216, 192, 164, 135, 115, 105, 96, 92, 95, 103, 109, 
  119, 115, 85, 43, 13, 12, 46, 85, 115, 133, 149, 161, 188, 236, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 191, 106, 51, 13, 22, 51, 82, 125, 177, 226, 250, 225, 164, 96, 22, 0, 0, 
  0, 0, 0, 24, 77, 125, 144, 157, 167, 183, 192, 168, 113, 53, 19, 17, 24, 19, 24, 47, 
  57, 57, 57, 72, 81, 72, 70, 58, 33, 12, 19, 37, 57, 99, 159, 216, 255, 255, 249, 216, 
  171, 149, 147, 147, 133, 103, 77, 55, 53, 92, 157, 240, 255, 255, 255, 255, 255, 255, 250, 208, 
  173, 149, 123, 101, 101, 115, 135, 147, 150, 153, 149, 147, 144, 149, 140, 143, 143, 144, 140, 140, 
  144, 140, 143, 143, 143, 143, 139, 140, 140, 140, 140, 137, 135, 137, 137, 137, 133, 130, 129, 130, 
  129, 129, 128, 128, 125, 125, 125, 128, 128, 128, 120, 120, 119, 123, 119, 116, 120, 116, 119, 119, 
  119, 115, 119, 116, 113, 115, 115, 115, 115, 113, 109, 106, 109, 113, 109, 109, 109, 109, 109, 109, 
  109, 109, 109, 109, 113, 113, 111, 115, 116, 116, 115, 115, 113, 96, 77, 75, 75, 67, 71, 105, 
  153, 183, 181, 154, 120, 87, 61, 48, 46, 48, 63, 87, 143, 212, 255, 255, 255, 255, 255, 235, 
  143, 77, 58, 67, 92, 92, 75, 57, 37, 24, 17, 15, 9, 0, 0, 0, 0, 0, 0, 3, 
  71, 159, 250, 255, 255, 255, 255, 240, 232, 229, 211, 171, 128, 95, 70, 55, 46, 43, 33, 3, 
  0, 33, 81, 113, 111, 96, 77, 61, 53, 53, 57, 75, 96, 120, 149, 177, 212, 255, 255, 255, 
  255, 255, 255, 255, 255, 183, 106, 43, 0, 0, 0, 0, 0, 0, 5, 67, 99, 105, 103, 96, 
  89, 82, 77, 79, 77, 79, 82, 89, 92, 99, 103, 109, 111, 115, 115, 116, 101, 55, 24, 7, 
  7, 24, 48, 75, 113, 147, 171, 202, 218, 236, 242, 255, 255, 255, 255, 255, 255, 255, 255, 250, 
  109, 9, 0, 0, 0, 0, 0, 23, 96, 130, 120, 89, 53, 19, 3, 0, 0, 0, 0, 3, 
  15, 24, 24, 0, 0, 9, 61, 109, 125, 120, 105, 101, 130, 178, 226, 245, 229, 198, 164, 139, 
  116, 103, 99, 92, 95, 99, 96, 89, 61, 36, 22, 31, 87, 159, 207, 215, 202, 174, 168, 177, 
  171, 154, 133, 129, 150, 154, 154, 157, 183, 218, 229, 225, 221, 225, 215, 178, 133, 89, 57, 29, 
  15, 9, 12, 17, 24, 31, 41, 43, 29, 37, 51, 63, 92, 133, 154, 149, 147, 135, 125, 123, 
  115, 113, 111, 109, 105, 106, 105, 89, 87, 115, 137, 149, 149, 144, 143, 137, 133, 129, 128, 125, 
  125, 123, 128, 125, 128, 128, 128, 128, 130, 128, 128, 125, 125, 129, 129, 130, 133, 129, 128, 128, 
  125, 125, 113, 113, 109, 115, 133, 143, 149, 144, 137, 135, 129, 120, 123, 113, 106, 113, 119, 119, 
  123, 120, 120, 119, 115, 119, 116, 113, 103, 75, 58, 72, 113, 149, 164, 183, 188, 181, 163, 143, 
  128, 116, 111, 103, 103, 103, 99, 103, 105, 105, 106, 106, 109, 109, 113, 111, 115, 116, 116, 116, 
  119, 116, 120, 119, 119, 119, 119, 119, 119, 113, 85, 55, 29, 33, 70, 103, 125, 135, 140, 137, 
  135, 135, 140, 171, 208, 236, 255, 255, 255, 255, 242, 202, 171, 129, 87, 71, 67, 55, 41, 48, 
  77, 113, 120, 125, 139, 144, 135, 103, 71, 43, 24, 15, 5, 0, 0, 0, 0, 19, 37, 67, 
  113, 174, 226, 245, 229, 211, 177, 149, 129, 111, 106, 95, 96, 99, 103, 109, 115, 119, 120, 99, 
  71, 77, 89, 95, 101, 105, 137, 187, 240, 255, 245, 211, 184, 178, 174, 163, 153, 163, 197, 218, 
  216, 216, 225, 225, 202, 159, 120, 95, 63, 48, 43, 39, 43, 48, 53, 61, 67, 71, 79, 85, 
  89, 91, 92, 87, 61, 36, 19, 12, 19, 33, 71, 129, 168, 192, 198, 205, 232, 255, 255, 255, 
  255, 255, 255, 222, 192, 171, 140, 92, 47, 17, 0, 0, 0, 0, 0, 0, 23, 63, 99, 111, 
  109, 92, 75, 75, 103, 154, 192, 202, 188, 178, 171, 154, 163, 178, 198, 211, 184, 139, 81, 27, 
  15, 12, 19, 53, 96, 143, 159, 154, 133, 115, 92, 67, 51, 46, 33, 23, 12, 13, 23, 39, 
  91, 163, 226, 255, 255, 255, 255, 255, 255, 242, 205, 154, 125, 103, 95, 96, 123, 150, 161, 147, 
  125, 109, 91, 85, 79, 79, 79, 85, 85, 91, 96, 101, 105, 109, 113, 115, 123, 120, 123, 128, 
  130, 119, 96, 58, 36, 19, 23, 39, 63, 89, 119, 150, 174, 198, 222, 255, 255, 255, 255, 255, 
  255, 255, 249, 168, 128, 111, 95, 101, 123, 150, 168, 154, 128, 95, 48, 2, 0, 0, 0, 0, 
  0, 13, 57, 92, 130, 150, 149, 137, 130, 135, 120, 92, 72, 53, 41, 33, 36, 33, 22, 17, 
  48, 91, 129, 144, 144, 149, 147, 147, 143, 140, 140, 139, 140, 143, 149, 149, 150, 153, 154, 157, 
  157, 159, 161, 161, 161, 161, 161, 161, 157, 161, 157, 153, 153, 150, 149, 147, 144, 147, 140, 135, 
  115, 79, 53, 31, 27, 37, 55, 77, 105, 133, 177, 232, 255, 255, 255, 255, 255, 255, 255, 212, 
  154, 101, 65, 57, 81, 128, 174, 215, 235, 231, 197, 129, 58, 13, 0, 0, 0, 0, 0, 0, 
  0, 0, 51, 135, 198, 249, 252, 229, 205, 188, 163, 128, 96, 70, 53, 46, 43, 46, 53, 61, 
  57, 51, 57, 53, 51, 53, 57, 79, 125, 181, 221, 229, 225, 211, 211, 226, 255, 255, 255, 255, 
  246, 208, 164, 130, 116, 125, 133, 133, 137, 150, 168, 168, 153, 135, 115, 105, 92, 87, 89, 89, 
  95, 101, 103, 109, 113, 116, 115, 116, 125, 133, 133, 128, 115, 103, 111, 128, 139, 149, 150, 150, 
  144, 144, 139, 143, 153, 163, 174, 184, 201, 202, 195, 178, 163, 150, 139, 120, 116, 113, 109, 101, 
  103, 113, 129, 133, 133, 137, 139, 130, 119, 106, 92, 82, 65, 41, 39, 43, 57, 70, 65, 65, 
  82, 111, 140, 168, 173, 163, 161, 159, 181, 188, 188, 191, 192, 187, 164, 137, 106, 91, 87, 87, 
  81, 82, 103, 111, 101, 96, 95, 103, 123, 139, 140, 130, 115, 101, 106, 116, 128, 154, 195, 240, 
  255, 255, 249, 242, 235, 211, 164, 119, 92, 65, 43, 17, 9, 5, 9, 9, 22, 41, 70, 96, 
  116, 115, 105, 95, 81, 71, 71, 65, 63, 72, 85, 105, 144, 164, 187, 198, 208, 231, 252, 252};
const unsigned char shoot[4080] = {
  129, 99, 103, 164, 214, 129, 31, 105, 204, 118, 55, 92, 140, 225, 152, 61, 84, 154, 184, 101, 
  75, 129, 209, 135, 47, 94, 125, 207, 166, 72, 79, 135, 195, 118, 68, 122, 205, 136, 64, 106, 
  143, 173, 105, 54, 122, 200, 133, 74, 106, 215, 236, 91, 43, 84, 163, 115, 34, 81, 150, 209, 
  184, 198, 191, 113, 95, 12, 0, 79, 167, 111, 120, 238, 255, 255, 118, 0, 0, 0, 0, 0, 
  89, 236, 255, 255, 255, 255, 190, 0, 0, 0, 20, 170, 161, 137, 255, 255, 255, 181, 37, 0, 
  0, 15, 0, 60, 167, 255, 255, 255, 255, 255, 60, 0, 0, 41, 60, 44, 128, 211, 255, 255, 
  255, 150, 51, 12, 0, 0, 10, 136, 201, 208, 236, 255, 255, 209, 0, 0, 0, 15, 16, 31, 
  128, 255, 255, 255, 255, 252, 144, 0, 0, 0, 77, 68, 75, 171, 255, 255, 255, 233, 139, 53, 
  0, 0, 9, 106, 77, 123, 209, 255, 255, 255, 146, 68, 7, 0, 0, 0, 136, 166, 159, 219, 
  255, 255, 255, 82, 0, 0, 0, 0, 0, 81, 214, 225, 229, 255, 255, 255, 115, 0, 0, 0, 
  75, 63, 47, 126, 209, 255, 255, 255, 222, 119, 54, 0, 0, 0, 112, 133, 103, 185, 255, 255, 
  255, 222, 136, 74, 0, 0, 0, 102, 96, 98, 177, 255, 255, 255, 255, 191, 119, 0, 0, 0, 
  44, 125, 116, 122, 222, 255, 255, 255, 236, 98, 50, 0, 0, 0, 17, 129, 208, 164, 194, 255, 
  255, 255, 198, 19, 0, 0, 0, 0, 40, 154, 205, 191, 209, 255, 255, 255, 147, 74, 20, 0, 
  0, 0, 112, 140, 119, 180, 255, 255, 255, 236, 211, 122, 0, 0, 0, 33, 85, 81, 123, 207, 
  255, 255, 255, 255, 255, 132, 0, 0, 0, 29, 31, 33, 115, 184, 255, 255, 248, 255, 255, 226, 
  33, 0, 0, 0, 55, 44, 58, 128, 215, 255, 243, 255, 255, 255, 226, 36, 0, 0, 0, 12, 
  31, 98, 170, 232, 225, 228, 255, 255, 255, 140, 39, 0, 0, 0, 0, 13, 96, 192, 192, 174, 
  242, 255, 255, 255, 207, 50, 0, 0, 0, 0, 0, 70, 149, 232, 192, 191, 248, 255, 255, 236, 
  140, 40, 0, 0, 0, 0, 24, 118, 208, 167, 163, 224, 255, 255, 255, 209, 137, 63, 10, 0, 
  0, 0, 88, 120, 115, 153, 181, 253, 255, 255, 228, 239, 219, 87, 0, 0, 0, 0, 22, 54, 
  130, 204, 200, 177, 238, 255, 255, 231, 208, 137, 75, 0, 0, 0, 0, 94, 105, 112, 178, 225, 
  204, 215, 255, 255, 255, 201, 112, 44, 10, 0, 0, 0, 91, 101, 115, 167, 214, 245, 232, 216, 
  255, 255, 225, 150, 63, 23, 0, 0, 0, 40, 101, 113, 140, 198, 216, 198, 219, 255, 255, 233, 
  180, 137, 81, 17, 0, 0, 0, 64, 78, 108, 154, 197, 240, 212, 207, 233, 255, 255, 209, 144, 
  71, 36, 0, 0, 0, 22, 103, 106, 136, 170, 200, 248, 221, 212, 233, 255, 255, 195, 112, 37, 
  6, 0, 0, 0, 27, 126, 128, 139, 173, 190, 224, 243, 221, 214, 233, 255, 221, 144, 54, 2, 
  0, 0, 0, 0, 67, 154, 177, 161, 173, 171, 214, 250, 221, 211, 225, 243, 204, 122, 48, 9, 
  3, 0, 0, 2, 75, 154, 167, 160, 177, 202, 218, 195, 195, 221, 249, 235, 188, 143, 105, 53, 
  0, 0, 0, 15, 63, 79, 133, 166, 202, 215, 174, 160, 191, 240, 229, 205, 200, 207, 204, 109, 
  20, 0, 0, 0, 0, 19, 89, 164, 174, 163, 168, 187, 216, 183, 171, 202, 211, 240, 212, 171, 
  154, 113, 68, 0, 0, 0, 0, 16, 54, 115, 152, 200, 214, 178, 157, 176, 201, 192, 177, 195, 
  218, 208, 168, 136, 87, 55, 2, 0, 0, 0, 55, 77, 108, 152, 180, 211, 198, 166, 170, 187, 
  195, 181, 185, 205, 218, 174, 149, 132, 91, 60, 0, 0, 0, 3, 64, 88, 108, 159, 198, 197, 
  181, 167, 177, 194, 173, 159, 184, 190, 214, 202, 160, 153, 140, 103, 34, 0, 0, 3, 6, 19, 
  71, 115, 166, 204, 184, 170, 183, 197, 166, 144, 152, 180, 204, 194, 174, 180, 185, 157, 132, 96, 
  60, 36, 0, 0, 0, 24, 78, 105, 125, 156, 185, 202, 177, 168, 168, 181, 170, 142, 154, 180, 
  201, 181, 167, 168, 171, 170, 128, 85, 44, 30, 16, 0, 3, 22, 58, 120, 137, 149, 173, 176, 
  194, 185, 164, 156, 156, 170, 146, 136, 161, 185, 191, 171, 156, 163, 168, 142, 108, 84, 60, 17, 
  0, 7, 29, 48, 55, 85, 128, 171, 183, 171, 178, 187, 178, 160, 147, 152, 160, 146, 133, 149, 
  168, 188, 170, 154, 159, 159, 156, 130, 109, 96, 68, 23, 5, 13, 23, 50, 71, 81, 106, 146, 
  168, 188, 191, 173, 171, 168, 174, 161, 139, 139, 147, 144, 132, 142, 159, 176, 174, 160, 153, 150, 
  147, 150, 128, 109, 91, 53, 44, 34, 15, 24, 40, 70, 82, 96, 126, 149, 180, 184, 178, 173, 
  174, 177, 166, 144, 144, 149, 150, 137, 126, 129, 143, 149, 153, 156, 163, 168, 149, 139, 136, 135, 
  136, 118, 106, 92, 74, 46, 26, 31, 44, 55, 60, 75, 91, 119, 146, 156, 166, 173, 183, 183, 
  168, 161, 159, 159, 156, 142, 139, 143, 143, 132, 128, 130, 139, 144, 137, 143, 152, 163, 160, 147, 
  140, 133, 136, 129, 116, 112, 118, 112, 89, 68, 50, 46, 53, 53, 58, 67, 84, 105, 105, 113, 
  136, 156, 170, 171, 173, 176, 177, 167, 160, 153, 153, 152, 140, 139, 142, 140, 140, 130, 132, 129, 
  137, 133, 128, 128, 135, 140, 142, 143, 147, 150, 146, 136, 130, 129, 126, 128, 116, 111, 113, 120, 
  119, 87, 46, 13, 0, 6, 53, 96, 139, 161, 200, 255, 255, 174, 82, 58, 55, 105, 219, 208, 
  82, 71, 70, 112, 235, 204, 79, 70, 71, 120, 239, 207, 79, 68, 67, 118, 242, 204, 78, 65, 
  65, 112, 242, 207, 78, 64, 60, 109, 243, 211, 77, 58, 57, 103, 245, 216, 81, 60, 58, 99, 
  232, 225, 82, 53, 58, 88, 221, 236, 98, 53, 58, 81, 205, 250, 118, 51, 55, 72, 183, 255, 
  139, 51, 57, 63, 167, 255, 164, 55, 54, 57, 143, 255, 191, 58, 53, 55, 115, 255, 214, 75, 
  51, 55, 96, 232, 243, 96, 44, 54, 78, 200, 255, 133, 47, 57, 63, 163, 255, 177, 53, 55, 
  55, 122, 255, 215, 64, 47, 51, 98, 238, 245, 98, 46, 60, 75, 191, 255, 150, 46, 53, 55, 
  142, 255, 201, 58, 48, 50, 101, 243, 243, 91, 44, 51, 70, 190, 255, 152, 46, 54, 58, 129, 
  255, 212, 68, 43, 50, 92, 222, 255, 116, 43, 54, 61, 164, 255, 187, 51, 46, 53, 102, 248, 
  246, 94, 41, 54, 65, 176, 255, 174, 48, 46, 54, 113, 255, 236, 81, 40, 54, 72, 177, 255, 
  168, 47, 48, 58, 111, 252, 245, 89, 46, 51, 71, 176, 255, 180, 50, 46, 54, 101, 235, 255, 
  106, 41, 54, 64, 152, 255, 207, 63, 39, 51, 88, 202, 255, 140, 40, 48, 55, 120, 255, 240, 
  82, 40, 53, 68, 163, 255, 191, 51, 46, 50, 94, 214, 255, 136, 40, 53, 57, 120, 255, 242, 
  88, 41, 54, 70, 156, 255, 207, 60, 43, 53, 85, 190, 255, 166, 46, 47, 60, 101, 222, 255, 
  125, 41, 48, 61, 122, 255, 243, 95, 37, 51, 70, 144, 255, 221, 67, 41, 54, 75, 164, 255, 
  200, 55, 43, 53, 84, 184, 255, 177, 50, 46, 57, 95, 201, 255, 156, 47, 47, 60, 101, 216, 
  255, 139, 44, 50, 61, 112, 224, 255, 126, 44, 51, 65, 113, 229, 255, 122, 39, 48, 63, 115, 
  236, 255, 120, 47, 48, 64, 119, 231, 255, 118, 43, 50, 68, 116, 229, 255, 128, 46, 47, 67, 
  112, 219, 255, 135, 47, 53, 63, 108, 205, 255, 154, 50, 48, 61, 98, 192, 255, 170, 50, 50, 
  58, 92, 173, 255, 195, 57, 39, 46, 77, 149, 255, 208, 65, 34, 51, 78, 137, 255, 233, 79, 
  41, 50, 72, 120, 232, 255, 112, 43, 47, 67, 106, 201, 255, 160, 47, 47, 60, 92, 170, 255, 
  200, 63, 46, 55, 82, 142, 255, 242, 91, 48, 50, 75, 118, 212, 255, 146, 48, 47, 63, 101, 
  167, 255, 201, 64, 47, 57, 84, 136, 243, 253, 106, 44, 55, 71, 111, 188, 255, 177, 55, 47, 
  55, 91, 146, 252, 243, 92, 47, 54, 77, 113, 191, 255, 168, 54, 48, 63, 92, 144, 250, 243, 
  96, 51, 50, 74, 106, 184, 255, 185, 57, 48, 54, 87, 133, 233, 255, 116, 50, 51, 65, 105, 
  164, 255, 214, 67, 48, 53, 84, 118, 200, 255, 160, 55, 51, 58, 95, 140, 242, 252, 108, 48, 
  51, 67, 101, 159, 255, 216, 71, 50, 50, 81, 113, 180, 255, 187, 57, 47, 53, 88, 122, 205, 
  255, 153, 50, 48, 61, 96, 135, 226, 255, 123, 51, 53, 65, 98, 144, 242, 240, 102, 48, 48, 
  72, 103, 146, 255, 232, 84, 50, 53, 74, 103, 156, 255, 222, 84, 51, 54, 79, 106, 159, 255, 
  224, 78, 53, 54, 75, 106, 154, 255, 226, 85, 51, 54, 75, 105, 152, 255, 228, 89, 51, 54, 
  74, 106, 147, 245, 238, 106, 50, 50, 74, 103, 139, 231, 250, 126, 53, 53, 65, 101, 129, 208, 
  255, 147, 54, 53, 61, 91, 120, 191, 255, 177, 61, 47, 57, 87, 115, 164, 255, 211, 71, 54, 
  53, 78, 109, 146, 243, 240, 108, 55, 55, 65, 105, 128, 202, 255, 156, 58, 51, 58, 98, 115, 
  171, 255, 208, 74, 51, 53, 79, 112, 143, 228, 253, 128, 55, 51, 64, 102, 122, 181, 255, 195, 
  67, 48, 57, 89, 111, 143, 233, 249, 123, 58, 55, 68, 102, 119, 176, 255, 195, 72, 51, 58, 
  84, 112, 136, 216, 255, 150, 55, 51, 60, 92, 115, 159, 253, 229, 99, 54, 58, 72, 103, 123, 
  178, 255, 195, 68, 53, 60, 81, 113, 136, 207, 255, 164, 61, 51, 58, 89, 119, 144, 225, 255, 
  137, 58, 54, 64, 94, 119, 154, 239, 240, 112, 54, 58, 68, 103, 119, 156, 246, 224, 96, 46, 
  48, 65, 101, 118, 159, 246, 224, 94, 50, 55, 65, 106, 122, 159, 248, 221, 99, 51, 55, 68, 
  105, 126, 156, 243, 228, 108, 55, 51, 70, 101, 122, 154, 231, 242, 128, 55, 55, 65, 98, 119, 
  144, 215, 255, 152, 60, 54, 63, 91, 119, 136, 194, 255, 181, 64, 51, 63, 85, 109, 126, 171, 
  255, 218, 94, 57, 58, 71, 108, 122, 153, 225, 245, 140, 57, 51, 67, 94, 118, 136, 187, 255, 
  192, 74, 53, 58, 82, 109, 126, 154, 229, 246, 130, 57, 57, 67, 95, 118, 132, 183, 255, 197, 
  77, 54, 60, 78, 106, 125, 150, 214, 255, 153, 61, 54, 63, 88, 116, 126, 166, 242, 226, 112, 
  58, 50, 67, 96, 120, 136, 180, 255, 202, 79, 54, 55, 71, 102, 122, 142, 195, 255, 181, 68, 
  53, 60, 79, 108, 125, 144, 198, 255, 171, 67, 50, 57, 85, 108, 126, 144, 202, 255, 168, 65, 
  50, 58, 85, 112, 128, 146, 197, 253, 173, 67, 50, 57, 82, 105, 123, 139, 190, 253, 185, 72, 
  50, 58, 79, 108, 126, 137, 177, 249, 207, 92, 58, 60, 72, 99, 119, 133, 161, 229, 236, 130, 
  58, 51, 67, 94, 113, 126, 147, 198, 252, 174, 67, 50, 61, 84, 105, 128, 136, 164, 243, 219, 
  103, 58, 53, 67, 96, 116, 130, 146, 198, 249, 170, 68, 50, 61, 79, 111, 126, 133, 163, 231, 
  229, 125, 58, 50, 64, 95, 112, 129, 140, 177, 248, 202, 92, 58, 54, 72, 99, 118, 132, 147, 
  190, 249, 187, 75, 53, 57, 81, 106, 118, 133, 144, 200, 255, 176, 67, 54, 60, 81, 109, 123, 
  137, 143, 197, 252, 173, 71, 53, 60, 81, 106, 118, 137, 143, 190, 250, 187, 77, 54, 63, 77, 
  102, 119, 133, 144, 177, 240, 202, 99, 54, 54, 74, 98, 118, 132, 137, 164, 224, 232, 132, 60, 
  48, 63, 94, 115, 126, 140, 152, 195, 249, 178, 72, 55, 55, 78, 109, 119, 135, 144, 168, 225, 
  228, 129, 63, 50, 63, 91, 112, 129, 139, 144, 188, 239, 200, 92, 60, 57, 71, 101, 119, 133, 
  143, 149, 197, 249, 180, 72, 53, 57, 70, 101, 113, 129, 136, 144, 197, 238, 167, 68, 50, 55, 
  77, 102, 118, 133, 140, 150, 197, 240, 170, 70, 57, 60, 81, 102, 116, 132, 142, 143, 190, 240, 
  187, 87, 53, 57, 72, 99, 118, 132, 143, 143, 173, 229, 212, 120, 60, 50, 68, 91, 115, 126, 
  136, 140, 156, 198, 239, 168, 72, 55, 58, 79, 106, 120, 133, 139, 143, 168, 224, 218, 132, 60, 
  48, 67, 91, 112, 125, 133, 143, 146, 176, 232, 204, 108, 55, 51, 71, 94, 113, 126, 139, 144, 
  144, 181, 232, 195, 103, 57, 53, 71, 95, 112, 129, 140, 140, 143, 174, 226, 204, 115, 57, 51, 
  71, 94, 112, 125, 135, 143, 143, 164, 209, 224, 146, 64, 51, 60, 79, 108, 123, 133, 140, 140, 
  150, 184, 229, 187, 89, 54, 54, 72, 96, 116, 129, 139, 137, 142, 160, 197, 229, 156, 68, 51, 
  55, 78, 106, 118, 130, 136, 140, 146, 161, 204, 221, 149, 64, 51, 63, 79, 105, 120, 129, 140, 
  142, 139, 160, 202, 221, 156, 64, 53, 60, 75, 102, 120, 130, 140, 144, 144, 154, 188, 224, 178, 
  89, 55, 54, 70, 95, 116, 125, 137, 143, 142, 142, 164, 204, 216, 144, 63, 51, 63, 84, 102, 
  118, 130, 136, 140, 143, 147, 171, 214, 198, 122, 60, 48, 65, 88, 112, 118, 132, 139, 139, 139, 
  143, 168, 209, 202, 128, 60, 50, 65, 87, 105, 120, 132, 139, 139, 143, 143, 160, 198, 212, 152, 
  70, 54, 60, 79, 101, 118, 130, 137, 137, 137, 142, 146, 174, 208, 194, 116, 57, 51, 71, 89, 
  111, 123, 128, 137, 140, 143, 147, 146, 177, 211, 184, 109, 57, 51, 70, 92, 111, 123, 136, 137, 
  137, 143, 143, 147, 171, 202, 198, 128, 61, 50, 63, 85, 108, 120, 132, 139, 140, 143, 143, 140, 
  152, 184, 208, 163, 89, 54, 54, 74, 96, 112, 128, 132, 140, 139, 143, 142, 140, 157, 184, 205, 
  159, 84, 58, 53, 72, 98, 116, 129, 139, 143, 142, 146, 140, 137, 147, 176, 201, 180, 111, 57, 
  51, 70, 92, 111, 125, 132, 140, 142, 144, 142, 140, 139, 154, 183, 205, 157, 84, 54, 51, 70, 
  92, 106, 125, 133, 136, 142, 140, 137, 136, 135, 144, 180, 200, 163, 98, 55, 50, 75, 89, 112, 
  125, 133, 139, 140, 143, 140, 139, 137, 135, 157, 188, 195, 144, 75, 55, 55, 81, 99, 115, 132, 
  137, 143, 142, 142, 139, 140, 136, 136, 153, 184, 197, 153, 84, 57, 55, 77, 98, 115, 128, 137, 
  142, 140, 144, 139, 136, 137, 139, 142, 161, 185, 185, 139, 74, 54, 64, 79, 102, 116, 129, 139, 
  139, 140, 144, 139, 140, 136, 135, 136, 150, 176, 192, 161, 96, 58, 53, 77, 96, 115, 123, 132, 
  142, 139, 139, 142, 142, 135, 137, 135, 130, 150, 177, 188, 160, 101, 58, 54, 72, 91, 111, 122, 
  135, 137, 143, 142, 142, 139, 136, 137, 135, 130, 133, 153, 176, 185, 149, 87, 61, 55, 77, 94, 
  112, 129, 135, 139, 139, 142, 140, 139, 136, 137, 135, 132, 128, 137, 161, 178, 183, 132, 75, 53, 
  60, 81, 101, 118, 130, 136, 140, 139, 143, 140, 135, 136, 137, 130, 129, 132, 128, 140, 167, 181, 
  167, 115, 70, 54, 67, 85, 108, 119, 136, 136, 142, 142, 139, 142, 140, 132, 135, 132, 130, 130, 
  126, 128, 147, 168, 183, 156, 106, 68, 57, 65, 92, 106, 123, 132, 136, 142, 140, 140, 137, 139, 
  133, 129, 133, 130, 125, 130, 130, 126, 146, 167, 176, 160, 113, 71, 55, 65, 87, 105, 119, 133, 
  139, 140, 140, 137, 140, 139, 130, 133, 130, 129, 132, 132, 128, 126, 130, 139, 154, 167, 174, 142, 
  95, 67, 57, 72, 95, 111, 126, 132, 140, 142, 140, 140, 139, 136, 137, 130, 135, 132, 128, 128, 
  128, 126, 130, 126, 132, 150, 163, 173, 161, 118, 79, 60, 67, 84, 102, 120, 129, 135, 140, 140, 
  142, 142, 140, 132, 135, 130, 130, 133, 129, 128, 126, 130, 130, 126, 125, 130, 137, 156, 163, 170, 
  146, 102, 74, 58, 72, 88, 109, 122, 130, 139, 140, 140, 139, 137, 136, 137, 130, 135, 132, 128, 
  129, 128, 132, 128, 126, 128, 125, 130, 130, 128, 135, 150, 159, 163, 163, 125, 91, 65, 63, 75, 
  95, 115, 123, 132, 140, 142, 143, 140, 139, 130, 132, 129, 128, 128, 123, 129, 129, 125, 123, 126, 
  126, 126, 125, 129, 125, 125, 125, 126, 135, 143, 154, 159, 157, 135, 102, 78, 61, 71, 89, 106, 
  120, 129, 137, 139, 143, 142, 142, 139, 139, 136, 132, 128, 133, 128, 128, 132, 128, 126, 132, 126, 
  126, 130, 128, 126, 126, 126, 128, 128, 128, 125, 129, 130, 139, 152, 152, 156, 150, 126, 99, 82, 
  65, 70, 89, 106, 120, 133, 139, 140, 143, 140, 139, 137, 139, 132, 136, 133, 129, 128, 133, 129, 
  128, 132, 128, 126, 125, 130, 128, 132, 128, 126, 130, 126, 126, 126, 125, 125, 125, 126, 128, 128, 
  128, 128, 128, 129, 142, 144, 150, 147, 144, 133, 108, 91, 79, 70, 77, 96, 106, 123, 133, 139, 
  144, 140, 140, 139, 137, 139, 132, 153, 176, 119, 102, 113, 113, 116, 130, 166, 122, 99, 130, 113, 
  132, 166, 118, 103, 130, 111, 136, 166, 115, 105, 129, 111, 132, 160, 120, 102, 128, 115, 130, 160, 
  120, 101, 129, 109, 130, 163, 119, 105, 128, 109, 130, 164, 116, 101, 129, 111, 130, 164, 116, 108, 
  129, 111, 128, 159, 119, 112, 129, 111, 125, 157, 120, 116, 128, 106, 126, 150, 130, 123, 120, 111, 
  120, 139, 146, 126, 113, 116, 119, 133, 153, 126, 106, 120, 113, 132, 160, 122, 102, 128, 109, 126, 
  163, 115, 111, 129, 109, 126, 154, 125, 119, 123, 111, 120, 139, 146, 126, 109, 120, 116, 133, 164, 
  118, 102, 128, 109, 126, 163, 119, 108, 129, 111, 125, 149, 130, 123, 118, 115, 122, 132, 159, 128, 
  108, 125, 111, 132, 164, 118, 109, 129, 111, 125, 150, 132, 128, 119, 112, 119, 132, 159, 123, 106, 
  128, 111, 128, 159, 120, 118, 125, 112, 122, 139, 150, 126, 111, 125, 115, 129, 164, 116, 112, 128, 
  111, 125, 144, 142, 128, 113, 118, 116, 130, 164, 120, 106, 126, 115, 126, 146, 139, 123, 113, 122, 
  116, 130, 166, 118, 106, 129, 112, 125, 144, 143, 129, 116, 119, 118, 130, 160, 116, 116, 125, 116, 
  126, 133, 154, 128, 109, 126, 116, 128, 153, 126, 120, 120, 118, 120, 132, 166, 122, 109, 126, 112, 
  120, 136, 139, 125, 109, 115, 118, 123, 154, 122, 116, 120, 112, 122, 129, 163, 119, 108, 123, 113, 
  125, 136, 152, 126, 112, 122, 118, 122, 150, 132, 125, 119, 116, 116, 130, 157, 119, 118, 123, 118, 
  122, 132, 159, 120, 109, 125, 115, 125, 137, 156, 123, 111, 123, 118, 126, 139, 142, 128, 116, 119, 
  118, 128, 147, 132, 122, 115, 118, 123, 128, 152, 128, 120, 125, 116, 116, 132, 153, 120, 120, 123, 
  118, 122, 128, 157, 122, 119, 125, 119, 123, 129, 159, 116, 116, 126, 119, 123, 129, 159, 116, 116, 
  126, 119, 120, 132, 159, 116, 118, 125, 119, 120, 132, 154, 118, 118, 123, 122, 123, 129, 153, 120, 
  120, 123, 118, 122, 128, 152, 120, 125, 125, 116, 123, 126, 144, 130, 120, 120, 113, 122, 126, 144, 
  137, 122, 118, 118, 123, 120, 135, 147, 125, 119, 119, 122, 120, 132, 154, 122, 115, 123, 118, 119, 
  132, 154, 119, 113, 123, 115, 126, 132, 146, 122, 125, 122, 115, 122, 125, 137, 143, 126, 119, 119, 
  125, 120, 132, 154, 122, 115, 126, 123, 122, 129, 153, 122, 119, 125, 119, 123, 126, 139, 142, 123, 
  119, 119, 125, 118, 132, 154, 118, 118, 125, 116, 126, 126, 140, 132, 125, 123, 116, 123, 120, 132, 
  154, 122, 113, 122, 118, 122, 128, 146, 126, 123, 122, 115, 126, 120, 132, 153, 116, 116, 123, 118, 
  122, 128, 142, 129, 123, 125, 116, 123, 120, 132, 153, 116, 116, 128, 116, 126, 126, 139, 137, 122, 
  120, 119, 122, 120, 132, 149, 120, 125, 125, 116, 129, 122, 129, 153, 122, 119, 125, 119, 128, 123, 
  136, 144, 128, 120, 118, 123, 123, 129, 142, 126, 122, 125, 119, 129, 118, 132, 149, 119, 123, 125, 
  119, 129, 122, 129, 153, 120, 120, 125, 119, 129, 128, 129, 150, 122, 122, 125, 120, 129, 125, 132, 
  139, 122, 120, 119, 125, 125, 129, 140, 137, 128, 120, 120, 126, 126, 126, 137, 137, 123, 122, 125, 
  125, 126, 129, 135, 137, 128, 120, 120, 123, 128, 128, 136, 137, 126, 120, 125, 125, 125, 125, 136, 
  140, 128, 120, 125, 125, 125, 125, 132, 144, 128, 115, 119, 116, 122, 123, 129, 144, 118, 115, 125, 
  118, 126, 120, 130, 144, 118, 122, 128, 116, 128, 122, 129, 140, 120, 125, 125, 119, 128, 123, 129, 
  136, 132, 125, 125, 122, 123, 128, 128, 129, 143, 123, 122, 125, 123, 128, 125, 125, 142, 120, 125, 
  125, 123, 128, 128, 128, 132, 136, 126, 122, 125, 123, 128, 125, 129, 143, 118, 125, 125, 123, 128, 
  128, 128, 132, 136, 126, 122, 125, 123, 128, 128, 129, 142, 119, 125, 125, 123, 128, 128, 128, 129, 
  137, 126, 122, 125, 125, 125, 126, 129, 142, 122, 126, 125, 123, 128, 125, 128, 129, 142, 119, 123, 
  125, 123, 123, 128, 128, 129, 137, 125, 125, 125, 123, 128, 128, 128, 132, 130, 123, 125, 125, 125, 
  126, 126, 126, 136, 126, 125, 125, 123, 123, 128, 128, 128, 137, 120, 123, 123, 123, 125, 129, 128, 
  128, 137, 120, 125, 125, 125, 123, 128, 128, 128, 136, 123, 123, 128, 119, 126, 128, 128, 128, 136, 
  123, 123, 128, 123, 123, 123, 123, 129, 136, 123, 125, 125, 119, 128, 128, 123, 130, 135, 129, 130, 
  120, 125, 125, 125, 125, 125, 129, 135, 125, 125, 125, 125, 123, 128, 128, 128, 137, 120, 123, 128, 
  125, 128, 128, 125, 125, 132, 125, 125, 125, 125, 126, 126, 126, 126, 126, 136, 126, 120, 126, 125, 
  123, 128, 128, 128, 136, 123, 125, 125, 123, 128, 123, 128, 128, 128, 133, 126, 125, 125, 125, 123, 
  128, 123, 128, 133, 123, 125, 125, 123, 128, 128, 128, 128, 128, 135, 123, 125, 125, 125, 125, 126, 
  126, 126, 130, 135, 125, 125, 125, 125, 125, 126, 126, 126, 130, 130, 125, 125, 125, 125, 126, 126, 
  126, 126, 130, 129, 125, 125, 125, 125, 126, 126, 126, 126, 132, 125, 125, 125, 125, 126, 126, 126, 
  126, 130, 130, 123, 129, 125, 125, 129, 125, 125, 125, 125, 129, 129, 125, 125, 125, 125, 126, 126, 
  126, 130, 129, 130, 126, 125, 125, 125, 125, 126, 129, 125, 129, 135, 125, 125, 129, 125, 125, 125, 
  129, 125, 129, 130, 123, 125, 129, 125, 129, 125, 125, 125, 129, 129, 130, 123, 125, 125, 125, 126, 
  126, 126, 122, 129, 128, 123, 123, 123, 122, 126, 122, 126, 128, 123, 128, 128, 128, 123, 128, 123, 
  128, 128, 128, 128, 128, 129, 129, 125, 129, 125, 125, 125, 125, 126, 126, 126, 130, 125, 125, 125, 
  126, 126, 126, 126, 128, 128, 128, 129, 125, 125, 125, 125, 126, 126, 126, 126, 126, 130, 129, 125, 
  125, 125, 125, 130, 125, 125, 129, 125, 129, 129, 125, 125, 125, 125, 126, 126, 126, 130, 125, 125, 
  129, 125, 125, 125, 125, 130, 125, 125, 125, 125, 130, 125, 129, 125, 129, 129, 125, 125, 129, 125, 
  129, 125, 129, 129, 125, 125, 125, 125, 129, 125, 125, 125, 126, 128, 128, 129, 125, 129, 125, 125};

const unsigned char invaderkilled[3377] = {
  128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 
  128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 
  128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 
  128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 
  128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 144, 164, 145, 111, 
  95, 91, 100, 106, 126, 134, 108, 109, 123, 160, 158, 118, 101, 144, 189, 110, 79, 154, 202, 117, 
  16, 96, 208, 187, 65, 100, 192, 175, 93, 0, 106, 253, 137, 17, 140, 242, 103, 1, 134, 255, 
  134, 4, 110, 230, 118, 1, 149, 255, 89, 6, 160, 255, 110, 0, 60, 177, 255, 81, 27, 207, 
  170, 43, 61, 200, 179, 23, 104, 245, 112, 11, 102, 210, 217, 39, 73, 237, 114, 29, 57, 173, 
  255, 91, 0, 145, 255, 138, 10, 62, 171, 252, 84, 0, 172, 244, 46, 47, 207, 190, 74, 0, 
  141, 255, 147, 16, 65, 191, 215, 30, 84, 224, 163, 78, 0, 128, 255, 124, 0, 149, 255, 102, 
  0, 138, 249, 169, 63, 0, 166, 255, 62, 24, 195, 229, 33, 78, 224, 163, 70, 0, 145, 255, 
  100, 0, 161, 255, 92, 0, 163, 255, 95, 0, 134, 246, 191, 77, 0, 125, 255, 130, 0, 88, 
  198, 226, 95, 0, 122, 255, 158, 43, 53, 213, 178, 21, 99, 214, 200, 49, 39, 198, 214, 75, 
  0, 131, 255, 134, 6, 104, 219, 179, 18, 105, 251, 104, 4, 106, 228, 204, 45, 51, 185, 247, 
  76, 0, 178, 244, 39, 52, 199, 220, 85, 0, 105, 255, 182, 35, 99, 254, 96, 0, 118, 207, 
  243, 84, 0, 137, 255, 124, 0, 140, 253, 121, 43, 19, 162, 255, 147, 15, 60, 180, 255, 62, 
  26, 196, 222, 91, 0, 135, 255, 96, 0, 178, 255, 76, 6, 179, 255, 80, 0, 166, 255, 108, 
  0, 110, 255, 92, 0, 122, 228, 215, 57, 24, 181, 252, 75, 0, 118, 255, 134, 1, 133, 214, 
  188, 65, 0, 163, 255, 95, 0, 143, 255, 126, 34, 48, 193, 255, 65, 22, 208, 201, 50, 31, 
  209, 199, 42, 70, 190, 251, 57, 11, 190, 221, 87, 0, 140, 255, 129, 27, 88, 209, 202, 54, 
  28, 186, 251, 94, 0, 144, 255, 82, 4, 190, 255, 60, 19, 202, 220, 42, 55, 212, 186, 35, 
  78, 221, 169, 27, 95, 213, 189, 75, 0, 146, 255, 113, 0, 169, 255, 83, 0, 118, 241, 164, 
  25, 107, 202, 212, 79, 0, 133, 255, 153, 42, 62, 216, 164, 7, 148, 229, 154, 54, 0, 185, 
  255, 65, 9, 197, 232, 54, 34, 189, 244, 80, 0, 113, 238, 210, 40, 90, 214, 159, 51, 2, 
  197, 218, 59, 31, 192, 255, 70, 8, 168, 255, 122, 0, 117, 235, 92, 7, 130, 234, 188, 16, 
  124, 229, 121, 46, 15, 178, 255, 104, 0, 142, 255, 87, 0, 156, 254, 131, 25, 100, 212, 183, 
  64, 6, 164, 255, 107, 8, 97, 179, 255, 99, 0, 121, 229, 155, 44, 87, 214, 181, 60, 35, 
  175, 255, 119, 15, 50, 167, 255, 98, 0, 171, 241, 113, 14, 97, 238, 127, 19, 159, 252, 91, 
  0, 162, 251, 126, 21, 102, 212, 172, 62, 15, 174, 255, 90, 0, 163, 229, 169, 49, 14, 189, 
  231, 78, 0, 147, 255, 127, 31, 120, 221, 148, 36, 86, 227, 127, 20, 131, 215, 195, 38, 70, 
  205, 178, 66, 0, 167, 255, 103, 5, 139, 215, 175, 73, 0, 176, 255, 122, 15, 104, 213, 164, 
  68, 0, 179, 255, 73, 22, 197, 245, 65, 0, 175, 255, 88, 0, 167, 242, 116, 39, 41, 198, 
  242, 67, 12, 171, 255, 105, 0, 149, 224, 142, 29, 88, 195, 198, 63, 0, 192, 241, 66, 10, 
  180, 255, 90, 0, 177, 250, 63, 24, 191, 236, 85, 0, 135, 247, 98, 29, 134, 222, 197, 50, 
  52, 200, 209, 58, 19, 196, 230, 59, 20, 175, 255, 103, 0, 158, 239, 80, 0, 148, 209, 206, 
  71, 0, 192, 234, 56, 28, 189, 255, 85, 0, 149, 237, 135, 51, 38, 174, 255, 92, 0, 145, 
  211, 150, 63, 7, 189, 255, 61, 17, 184, 255, 119, 21, 52, 182, 240, 63, 0, 190, 245, 86, 
  4, 126, 229, 169, 62, 55, 195, 246, 55, 13, 192, 243, 65, 0, 181, 251, 118, 42, 38, 187, 
  255, 68, 0, 186, 255, 131, 45, 29, 182, 255, 83, 0, 148, 211, 187, 81, 0, 181, 255, 84, 
  9, 158, 240, 91, 11, 181, 249, 99, 19, 117, 229, 107, 33, 121, 215, 206, 38, 87, 213, 153, 
  47, 47, 211, 177, 32, 119, 225, 128, 24, 145, 236, 91, 31, 121, 208, 215, 61, 0, 181, 255, 
  85, 2, 165, 228, 99, 28, 92, 213, 194, 39, 108, 211, 164, 66, 0, 189, 255, 109, 42, 53, 
  202, 227, 62, 20, 179, 255, 99, 20, 97, 192, 219, 59, 1, 190, 255, 84, 23, 83, 186, 255, 
  62, 0, 191, 236, 66, 0, 193, 242, 59, 28, 208, 212, 56, 30, 181, 255, 65, 8, 198, 221, 
  46, 31, 204, 203, 42, 63, 213, 176, 53, 49, 194, 251, 60, 6, 167, 246, 92, 19, 133, 199, 
  211, 60, 0, 170, 252, 126, 32, 104, 209, 149, 41, 81, 204, 192, 42, 74, 218, 159, 49, 73, 
  193, 250, 78, 0, 170, 235, 67, 15, 177, 252, 87, 31, 126, 195, 243, 68, 0, 180, 230, 64, 
  8, 158, 242, 130, 40, 109, 191, 223, 64, 0, 170, 228, 151, 47, 38, 200, 213, 63, 0, 152, 
  243, 167, 39, 124, 222, 105, 54, 43, 190, 255, 57, 14, 175, 235, 142, 28, 91, 194, 171, 46, 
  23, 201, 240, 69, 7, 143, 239, 145, 34, 157, 238, 79, 41, 86, 190, 255, 59, 0, 191, 234, 
  112, 40, 43, 197, 248, 57, 3, 194, 255, 70, 17, 191, 232, 83, 13, 107, 209, 190, 54, 41, 
  209, 217, 52, 25, 205, 239, 54, 20, 192, 251, 113, 29, 43, 190, 253, 52, 0, 145, 235, 165, 
  34, 133, 224, 116, 33, 94, 212, 165, 33, 99, 230, 109, 23, 182, 233, 110, 32, 106, 218, 148, 
  30, 128, 232, 86, 15, 191, 240, 96, 42, 73, 201, 252, 45, 6, 208, 238, 53, 0, 157, 247, 
  130, 41, 104, 190, 243, 42, 0, 176, 222, 140, 44, 6, 201, 255, 57, 0, 169, 216, 175, 49, 
  16, 217, 203, 32, 72, 213, 185, 45, 51, 226, 176, 36, 89, 201, 250, 69, 0, 177, 227, 53, 
  21, 152, 245, 149, 39, 94, 200, 250, 59, 0, 134, 216, 146, 27, 144, 224, 121, 45, 46, 214, 
  240, 31, 64, 225, 157, 40, 91, 205, 206, 43, 16, 212, 254, 75, 0, 160, 236, 78, 37, 118, 
  215, 215, 35, 68, 209, 196, 46, 2, 209, 241, 60, 1, 174, 249, 80, 38, 139, 213, 221, 43, 
  0, 182, 242, 134, 20, 100, 205, 161, 49, 5, 209, 255, 44, 34, 223, 219, 44, 21, 187, 255, 
  64, 25, 136, 225, 168, 15, 134, 204, 139, 34, 36, 210, 219, 50, 6, 216, 237, 34, 58, 209, 
  222, 53, 0, 188, 230, 140, 36, 65, 227, 142, 41, 86, 218, 211, 29, 82, 218, 165, 37, 51, 
  206, 239, 34, 3, 220, 214, 41, 33, 203, 255, 54, 4, 172, 232, 98, 25, 145, 212, 137, 28, 
  93, 209, 173, 42, 23, 224, 207, 27, 94, 219, 179, 50, 6, 198, 255, 97, 26, 149, 222, 92, 
  34, 103, 212, 212, 39, 27, 211, 252, 69, 0, 160, 225, 89, 30, 132, 219, 163, 37, 75, 218, 
  195, 39, 36, 228, 196, 32, 84, 226, 152, 25, 130, 209, 151, 39, 48, 228, 167, 40, 69, 209, 
  251, 38, 22, 214, 224, 35, 2, 170, 241, 104, 26, 161, 204, 178, 41, 0, 207, 250, 90, 23, 
  118, 224, 112, 33, 144, 222, 137, 51, 53, 206, 254, 55, 4, 183, 214, 132, 46, 5, 213, 255, 
  42, 30, 172, 231, 167, 32, 45, 213, 202, 17, 86, 227, 113, 49, 85, 213, 252, 29, 32, 229, 
  189, 40, 32, 191, 254, 83, 13, 165, 220, 73, 35, 108, 222, 208, 34, 63, 219, 213, 35, 14, 
  195, 235, 137, 43, 0, 201, 255, 45, 34, 159, 229, 161, 30, 83, 220, 162, 41, 54, 205, 248, 
  38, 19, 177, 224, 170, 31, 25, 226, 198, 31, 71, 215, 236, 38, 0, 206, 237, 52, 31, 216, 
  217, 34, 22, 218, 249, 74, 16, 156, 222, 62, 28, 216, 241, 68, 17, 142, 220, 89, 19, 191, 
  227, 81, 42, 84, 207, 255, 35, 0, 180, 207, 157, 33, 51, 223, 180, 38, 42, 226, 230, 23, 
  63, 222, 188, 40, 9, 205, 255, 66, 45, 107, 209, 252, 26, 0, 213, 227, 84, 18, 140, 213, 
  132, 51, 24, 208, 255, 65, 39, 213, 222, 40, 26, 115, 215, 242, 27, 21, 224, 225, 48, 11, 
  155, 229, 123, 47, 119, 220, 168, 23, 121, 212, 129, 46, 25, 204, 255, 77, 29, 170, 206, 124, 
  46, 24, 202, 255, 63, 37, 208, 219, 46, 17, 166, 223, 91, 35, 179, 228, 70, 41, 135, 215, 
  214, 22, 20, 230, 192, 28, 83, 214, 191, 30, 32, 227, 207, 30, 71, 229, 169, 40, 61, 199, 
  248, 70, 19, 153, 205, 123, 42, 78, 221, 193, 22, 104, 209, 169, 42, 6, 215, 246, 58, 41, 
  203, 218, 46, 17, 192, 221, 73, 41, 135, 227, 154, 45, 75, 202, 242, 59, 12, 173, 201, 45, 
  44, 149, 232, 171, 40, 72, 208, 230, 27, 10, 215, 203, 38, 45, 217, 226, 30, 31, 217, 210, 
  36, 26, 202, 223, 134, 45, 34, 227, 213, 32, 65, 221, 194, 36, 47, 198, 231, 115, 24, 93, 
  197, 170, 32, 54, 228, 179, 44, 56, 198, 238, 128, 44, 39, 213, 227, 23, 44, 218, 212, 46, 
  20, 163, 220, 104, 61, 101, 212, 245, 43, 26, 106, 201, 236, 30, 15, 213, 218, 55, 46, 122, 
  214, 203, 24, 73, 215, 163, 37, 66, 224, 174, 42, 69, 198, 235, 92, 42, 84, 200, 226, 27, 
  23, 212, 209, 65, 38, 151, 217, 98, 45, 188, 210, 94, 61, 70, 210, 242, 65, 47, 73, 203, 
  247, 27, 38, 145, 206, 219, 30, 16, 155, 224, 152, 44, 116, 203, 177, 37, 28, 198, 227, 82, 
  47, 153, 199, 123, 44, 103, 219, 130, 42, 169, 197, 120, 52, 41, 188, 239, 80, 49, 181, 206, 
  68, 45, 163, 196, 166, 35, 37, 227, 180, 41, 92, 221, 170, 41, 101, 211, 172, 46, 40, 199, 
  223, 53, 60, 138, 218, 182, 27, 106, 201, 130, 49, 49, 188, 234, 115, 46, 137, 203, 88, 49, 
  174, 196, 119, 53, 65, 210, 210, 31, 32, 194, 214, 72, 51, 198, 199, 50, 43, 167, 208, 79, 
  50, 188, 205, 85, 49, 136, 193, 160, 43, 33, 217, 220, 79, 55, 103, 206, 199, 36, 49, 219, 
  189, 41, 76, 213, 198, 36, 38, 183, 216, 99, 62, 89, 202, 234, 47, 40, 167, 198, 94, 45, 
  182, 194, 90, 60, 69, 214, 218, 44, 77, 210, 188, 33, 45, 139, 213, 205, 36, 50, 217, 191, 
  42, 43, 185, 215, 90, 69, 100, 204, 220, 34, 61, 208, 186, 41, 37, 168, 206, 128, 57, 113, 
  205, 172, 41, 68, 220, 163, 50, 83, 206, 215, 48, 41, 162, 194, 124, 62, 70, 190, 224, 65, 
  59, 205, 184, 41, 52, 208, 199, 61, 49, 145, 202, 112, 61, 147, 202, 142, 47, 105, 204, 147, 
  45, 100, 207, 156, 47, 89, 206, 176, 33, 106, 205, 116, 64, 85, 197, 218, 62, 59, 124, 200, 
  179, 33, 92, 207, 137, 49, 110, 194, 170, 40, 59, 219, 168, 47, 93, 206, 186, 39, 52, 149, 
  213, 184, 37, 66, 209, 191, 45, 42, 202, 184, 50, 73, 133, 224, 186, 35, 112, 191, 150, 45, 
  42, 201, 206, 70, 62, 160, 194, 118, 60, 105, 207, 172, 46, 67, 205, 195, 44, 72, 206, 181, 
  44, 49, 181, 200, 107, 66, 116, 198, 174, 43, 51, 164, 216, 121, 51, 175, 179, 89, 61, 95, 
  210, 158, 53, 100, 205, 186, 39, 51, 169, 206, 119, 65, 88, 183, 211, 59, 60, 200, 176, 59, 
  61, 116, 203, 190, 41, 75, 212, 177, 46, 59, 197, 196, 65, 68, 164, 195, 105, 57, 168, 181, 
  87, 66, 109, 201, 191, 40, 51, 192, 194, 120, 64, 80, 205, 203, 50, 54, 195, 182, 79, 73, 
  99, 210, 190, 50, 83, 203, 192, 45, 56, 174, 188, 86, 65, 196, 176, 46, 79, 208, 169, 49, 
  68, 193, 198, 100, 70, 84, 202, 199, 44, 65, 125, 207, 181, 51, 81, 200, 190, 47, 60, 172, 
  184, 84, 67, 188, 185, 84, 65, 144, 195, 107, 73, 111, 194, 206, 60, 61, 149, 184, 111, 60, 
  167, 177, 104, 69, 80, 206, 192, 58, 79, 199, 173, 55, 83, 140, 205, 175, 29, 47, 181, 184, 
  78, 83, 102, 195, 204, 54, 73, 107, 198, 187, 47, 78, 144, 205, 130, 60, 94, 189, 193, 66, 
  65, 114, 196, 152, 52, 135, 182, 137, 58, 68, 207, 196, 78, 69, 154, 182, 93, 73, 157, 187, 
  104, 77, 103, 182, 207, 69, 72, 192, 170, 64, 63, 166, 177, 90, 85, 105, 188, 204, 67, 75, 
  198, 171, 64, 76, 104, 198, 203, 65, 73, 121, 190, 177, 43, 72, 157, 197, 167, 50, 65, 202, 
  164, 66, 88, 145, 210, 138, 63, 95, 184, 190, 74, 68, 111, 191, 184, 42, 71, 201, 172, 57, 
  82, 149, 198, 174, 47, 64, 187, 180, 71, 87, 193, 159, 56, 82, 195, 173, 62, 82, 184, 179, 
  75, 75, 156, 184, 104, 69, 148, 181, 105, 74, 116, 192, 178, 50, 72, 168, 183, 97, 76, 139, 
  178, 150, 51, 92, 196, 153, 55, 80, 195, 183, 65, 87, 195, 147, 66, 89, 167, 191, 104, 76, 
  108, 191, 162, 53, 82, 189, 178, 67, 79, 117, 194, 189, 48, 70, 150, 185, 116, 69, 161, 166, 
  107, 78, 89, 196, 185, 57, 86, 192, 164, 62, 82, 169, 182, 96, 76, 157, 170, 121, 65, 114, 
  189, 117, 78, 108, 171, 197, 116, 60, 115, 180, 125, 71, 97, 189, 181, 60, 89, 195, 162, 63, 
  76, 149, 184, 120, 70, 162, 168, 80, 84, 155, 184, 105, 80, 110, 175, 184, 68, 83, 150, 177, 
  152, 49, 71, 187, 175, 96, 79, 97, 184, 181, 71, 84, 170, 171, 86, 84, 131, 192, 167, 52, 
  77, 167, 178, 111, 75, 91, 179, 185, 87, 81, 141, 179, 114, 83, 109, 173, 188, 85, 82, 112, 
  186, 170, 55, 91, 192, 163, 60, 78, 157, 181, 109, 84, 138, 185, 149, 61, 83, 171, 182, 117, 
  74, 88, 193, 174, 67, 86, 120, 191, 180, 52, 85, 168, 174, 90, 82, 180, 156, 85, 85, 111, 
  193, 164, 65, 99, 173, 176, 107, 71, 96, 187, 162, 65, 98, 184, 163, 72, 90, 185, 165, 72, 
  83, 156, 175, 123, 73, 116, 189, 133, 80, 115, 188, 159, 59, 86, 133, 197, 157, 63, 109, 182, 
  157, 58, 82, 157, 178, 121, 75, 92, 149, 194, 131, 68, 87, 159, 182, 106, 77, 121, 179, 135, 
  74, 107, 180, 160, 65, 91, 169, 169, 104, 78, 99, 179, 173, 73, 95, 185, 160, 74, 89, 128, 
  182, 163, 57, 93, 190, 159, 70, 86, 175, 159, 82, 98, 146, 186, 153, 63, 83, 180, 166, 86, 
  88, 144, 171, 121, 82, 106, 184, 164, 74, 93, 157, 171, 104, 84, 129, 181, 148, 64, 92, 138, 
  192, 166, 55, 87, 184, 152, 70, 93, 159, 176, 105, 88, 155, 160, 114, 80, 124, 181, 122, 82, 
  127, 179, 150, 66, 88, 173, 171, 87, 94, 151, 171, 133, 69, 106, 185, 131, 77, 103, 141, 197, 
  137, 73, 127, 167, 132, 70, 86, 163, 177, 105, 89, 134, 171, 133, 74, 106, 186, 143, 77, 105, 
  172, 166, 79, 88, 142, 170, 122, 80, 105, 146, 190, 135, 71, 95, 174, 156, 73, 98, 126, 186, 
  159, 65, 90, 141, 178, 139, 75, 101, 173, 156, 74, 98, 156, 168, 123, 75, 115, 176, 143, 69, 
  101, 185, 145, 75, 106, 183, 149, 75, 97, 160, 164, 96, 96, 167, 154, 101, 82, 140, 166, 100, 
  100, 119, 159, 179, 92, 94, 130, 176, 155, 63, 86, 157, 164, 104, 92, 133, 175, 143, 72, 102, 
  170, 150, 85, 99, 163, 165, 100, 91, 112, 149, 185, 117, 80, 129, 162, 111, 87, 140, 164, 111, 
  92, 142, 163, 128, 78, 113, 176, 141, 76, 98, 173, 162, 106, 91, 114, 179, 148, 74, 105, 163, 
  163, 100, 89, 145, 153, 99, 100, 136, 177, 127, 86, 114, 138, 184, 137, 69, 94, 163, 160, 93, 
  99, 122, 177, 156, 72, 94, 150, 164, 109, 96, 136, 171, 131, 82, 105, 147, 175, 105, 91, 151, 
  150, 104, 96, 107, 146, 184, 118, 88, 144, 152, 108, 95, 119, 177, 149, 72, 102, 124, 176, 165, 
  84, 96, 136, 165, 130, 80, 108, 171, 151, 82, 104, 141, 170, 133, 83, 103, 150, 172, 130, 81, 
  100, 169, 157, 98, 99, 113, 169, 159, 85, 107, 150, 162, 120, 87, 112, 171, 151, 77, 103, 169, 
  144, 93, 102, 139, 178, 124, 94, 115, 133, 181, 134, 77, 102, 162, 153, 93, 95, 114, 166, 150, 
  80, 105, 145, 163, 114, 90, 146, 147, 100, 101, 131, 171, 138, 79, 107, 128, 178, 152, 78, 95, 
  135, 163, 130, 86, 115, 167, 143, 88, 108, 154, 158, 117, 89, 108, 170, 153, 89, 102, 152, 148, 
  112, 99, 113, 173, 146, 90, 109, 143, 170, 121, 89, 108, 143, 171, 130, 82, 108, 170, 139, 80, 
  107, 130, 173, 158, 85, 106, 127, 169, 153, 80, 101, 148, 154, 109, 100, 136, 160, 121, 94, 117, 
  163, 147, 93, 105, 145, 160, 110, 100, 119, 160, 157, 96, 103, 117, 168, 147, 81, 105, 139, 165, 
  136, 86, 103, 158, 154, 101, 105, 147, 156, 115, 98, 116, 156, 153, 91, 107, 161, 145, 93, 109, 
  116, 147, 174, 106, 106, 118, 138, 168, 107, 95, 137, 149, 116, 96, 114, 163, 148, 93, 107, 133, 
  163, 139, 83, 105, 160, 147, 96, 110, 153, 146, 105, 105, 117, 141, 173, 133, 84, 113, 163, 132, 
  86, 109, 126, 165, 149, 97, 108, 127, 166, 146, 81, 108, 146, 138, 103, 107, 153, 144, 104, 109, 
  132, 164, 130, 97, 116, 152, 149, 102, 106, 129, 164, 137, 83, 105, 141, 160, 125, 96, 110, 140, 
  167, 121, 101, 116, 155, 145, 94, 108, 135, 159, 132, 88, 108, 138, 165, 130, 94, 118, 160, 137, 
  91, 108, 138, 156, 130, 97, 114, 160, 143, 91, 112, 150, 141, 102, 109, 150, 145, 104, 108, 150, 
  142, 109, 108, 127, 163, 136, 91, 110, 134, 161, 132, 96, 112, 148, 149, 111, 101, 130, 153, 125, 
  99, 113, 137, 168, 136, 90, 108, 143, 152, 115, 106, 125, 153, 138, 93, 122, 154, 127, 99, 125, 
  158, 119, 103, 116, 138, 167, 122, 97, 115, 149, 145, 105, 108, 136, 146, 113, 109, 118, 133, 172, 
  133, 93, 109, 139, 150, 113, 108, 134, 147, 128, 97, 112, 151, 147, 105, 115, 126, 142, 153, 107, 
  107, 122, 148, 147, 101, 108, 130, 153, 127, 101, 120, 148, 139, 106, 113, 127, 160, 139, 97, 109, 
  136, 152, 123, 105, 126, 154, 138, 94, 111, 139, 149, 123, 105, 119, 152, 143, 106, 110, 135, 146, 
  122, 105, 118, 153, 143, 108, 113, 116, 145, 145, 105, 111, 124, 153, 135, 93, 114, 150, 134, 102, 
  117, 124, 148, 150, 109, 112, 123, 151, 138, 96, 112, 124, 152, 142, 105, 112, 125, 154, 133, 102, 
  116, 126, 154, 145, 108, 111, 115, 141, 150, 110, 116, 122, 133, 158, 115, 111, 128};

const unsigned char highpitch[1802] = {
  255, 162, 102, 101, 46, 7, 47, 59, 111, 150, 160, 176, 163, 226, 220, 199, 157, 120, 74, 95, 
  31, 13, 47, 64, 116, 155, 162, 171, 181, 246, 207, 194, 142, 102, 84, 70, 8, 31, 53, 92, 
  138, 156, 174, 162, 223, 226, 201, 167, 124, 79, 95, 18, 18, 47, 71, 119, 154, 170, 159, 206, 
  239, 203, 183, 136, 85, 101, 25, 9, 43, 62, 114, 155, 166, 164, 206, 239, 204, 183, 136, 87, 
  97, 24, 10, 44, 71, 114, 153, 166, 163, 222, 233, 203, 172, 125, 89, 78, 9, 23, 48, 91, 
  124, 163, 164, 183, 249, 212, 198, 147, 93, 96, 36, 6, 42, 64, 110, 152, 166, 168, 227, 228, 
  203, 161, 113, 97, 58, 0, 37, 52, 104, 141, 167, 162, 221, 236, 204, 171, 116, 99, 57, 0, 
  35, 53, 104, 139, 167, 162, 221, 236, 204, 171, 113, 101, 54, 0, 37, 54, 106, 149, 157, 176, 
  238, 225, 202, 155, 106, 98, 22, 8, 42, 76, 117, 160, 152, 209, 248, 209, 185, 129, 107, 59, 
  0, 31, 51, 104, 142, 156, 180, 244, 222, 201, 152, 109, 86, 10, 13, 45, 92, 123, 160, 159, 
  234, 233, 204, 162, 119, 92, 19, 8, 43, 81, 125, 162, 155, 232, 236, 206, 166, 115, 92, 19, 
  8, 43, 88, 127, 156, 170, 242, 227, 204, 149, 118, 71, 2, 19, 47, 102, 143, 148, 201, 252, 
  213, 195, 128, 116, 38, 0, 36, 60, 107, 148, 149, 232, 245, 209, 153, 122, 66, 4, 22, 47, 
  93, 139, 145, 216, 250, 212, 175, 123, 92, 16, 8, 43, 90, 131, 143, 203, 253, 215, 188, 130, 
  102, 21, 5, 40, 86, 129, 144, 206, 253, 215, 183, 135, 85, 19, 7, 42, 97, 139, 145, 219, 
  248, 210, 163, 143, 60, 2, 19, 54, 106, 140, 168, 243, 233, 206, 144, 126, 32, 0, 39, 77, 
  126, 141, 210, 252, 213, 174, 143, 59, 2, 19, 59, 109, 134, 183, 250, 227, 196, 143, 99, 16, 
  6, 44, 99, 135, 163, 236, 238, 203, 150, 116, 19, 3, 40, 91, 127, 161, 234, 241, 202, 153, 
  110, 21, 3, 40, 96, 122, 172, 237, 238, 194, 150, 92, 9, 8, 50, 104, 126, 193, 255, 222, 
  175, 154, 53, 0, 31, 64, 116, 140, 216, 252, 206, 164, 124, 24, 1, 47, 92, 112, 184, 252, 
  230, 182, 151, 48, 0, 40, 64, 116, 155, 228, 247, 192, 163, 89, 9, 13, 52, 109, 132, 215, 
  253, 202, 168, 107, 16, 5, 50, 105, 128, 211, 254, 199, 171, 113, 17, 4, 54, 108, 125, 216, 
  253, 197, 171, 94, 13, 18, 55, 109, 142, 227, 249, 184, 169, 62, 0, 47, 70, 107, 182, 247, 
  224, 185, 137, 28, 0, 49, 100, 125, 220, 252, 190, 172, 72, 0, 46, 68, 104, 189, 252, 213, 
  182, 117, 21, 6, 64, 99, 157, 237, 239, 188, 140, 36, 0, 57, 98, 126, 233, 248, 185, 161, 
  38, 0, 56, 90, 130, 229, 249, 189, 154, 33, 0, 60, 93, 139, 231, 246, 181, 148, 35, 0, 
  67, 90, 159, 240, 223, 195, 114, 24, 11, 71, 97, 192, 255, 195, 190, 68, 0, 57, 72, 126, 
  221, 252, 189, 138, 42, 0, 73, 87, 188, 255, 195, 191, 60, 0, 69, 69, 143, 235, 226, 199, 
  107, 21, 30, 66, 112, 215, 255, 187, 135, 32, 1, 75, 99, 205, 255, 184, 159, 39, 0, 74, 
  89, 204, 255, 189, 160, 38, 0, 83, 93, 206, 255, 188, 146, 36, 9, 72, 102, 217, 255, 195, 
  113, 18, 39, 66, 134, 231, 227, 201, 66, 0, 72, 69, 181, 255, 194, 172, 44, 0, 61, 103, 
  212, 240, 193, 87, 7, 58, 60, 176, 247, 193, 162, 31, 13, 69, 127, 224, 227, 188, 55, 0, 
  72, 95, 209, 247, 197, 78, 4, 65, 71, 202, 253, 198, 99, 12, 61, 65, 193, 249, 199, 105, 
  19, 60, 67, 195, 247, 200, 97, 14, 64, 77, 202, 252, 187, 80, 7, 63, 108, 206, 252, 176, 
  60, 3, 68, 144, 219, 233, 143, 43, 30, 66, 187, 244, 195, 87, 10, 65, 109, 207, 246, 152, 
  58, 16, 68, 180, 243, 199, 84, 12, 65, 122, 214, 240, 130, 43, 44, 76, 195, 246, 167, 63, 
  13, 66, 178, 243, 189, 76, 12, 64, 164, 230, 209, 82, 18, 64, 147, 224, 217, 88, 17, 65, 
  143, 222, 211, 86, 23, 64, 151, 231, 204, 79, 21, 68, 169, 236, 177, 69, 24, 74, 192, 246, 
  146, 55, 37, 93, 206, 231, 103, 33, 59, 144, 228, 191, 74, 25, 83, 184, 251, 116, 38, 61, 
  133, 221, 185, 79, 26, 93, 193, 238, 101, 33, 70, 155, 244, 134, 60, 45, 125, 222, 178, 84, 
  23, 105, 192, 225, 93, 23, 96, 174, 249, 94, 28, 92, 164, 249, 95, 38, 81, 155, 250, 103, 
  40, 88, 157, 244, 95, 35, 95, 162, 243, 89, 25, 114, 176, 219, 86, 19, 135, 207, 162, 87, 
  38, 148, 236, 107, 59, 86, 160, 233, 87, 22, 136, 201, 157, 86, 50, 159, 233, 91, 30, 127, 
  198, 159, 81, 55, 165, 230, 86, 23, 145, 206, 127, 63, 101, 176, 183, 80, 51, 169, 220, 81, 
  31, 158, 216, 100, 38, 143, 199, 120, 54, 123, 197, 138, 61, 112, 185, 147, 64, 102, 190, 147, 
  68, 106, 180, 145, 65, 113, 194, 130, 54, 132, 200, 105, 47, 158, 201, 91, 44, 181, 197, 75, 
  66, 189, 165, 58, 121, 191, 103, 50, 171, 198, 75, 68, 194, 152, 53, 151, 188, 85, 53, 192, 
  163, 52, 150, 183, 85, 59, 201, 141, 46, 176, 174, 68, 102, 198, 98, 50, 205, 146, 45, 188, 
  162, 59, 141, 181, 79, 90, 199, 98, 59, 213, 116, 51, 210, 126, 46, 206, 138, 46, 187, 160, 
  56, 150, 178, 78, 91, 198, 105, 50, 199, 140, 46, 153, 178, 68, 77, 198, 123, 37, 184, 176, 
  62, 93, 190, 119, 46, 175, 189, 66, 75, 194, 147, 53, 135, 193, 98, 46, 171, 200, 76, 51, 
  184, 189, 70, 79, 186, 165, 61, 102, 193, 142, 65, 115, 190, 140, 60, 109, 187, 149, 67, 99, 
  189, 163, 72, 77, 181, 191, 78, 44, 177, 221, 78, 31, 157, 216, 106, 53, 124, 186, 167, 79, 
  52, 173, 227, 73, 30, 150, 209, 135, 72, 71, 170, 231, 73, 29, 138, 200, 171, 77, 43, 161, 
  233, 102, 63, 82, 163, 246, 75, 32, 116, 178, 223, 72, 21, 131, 197, 197, 82, 18, 139, 206, 
  184, 78, 23, 134, 208, 188, 80, 21, 127, 201, 203, 77, 21, 109, 194, 236, 77, 31, 91, 177, 
  253, 98, 46, 60, 150, 240, 147, 66, 37, 112, 212, 217, 80, 25, 78, 181, 244, 138, 51, 42, 
  113, 209, 223, 82, 27, 70, 169, 247, 166, 56, 23, 83, 209, 246, 125, 46, 46, 102, 213, 233, 
  93, 33, 66, 123, 220, 215, 81, 26, 70, 140, 225, 210, 81, 23, 70, 138, 226, 220, 89, 32, 
  65, 119, 219, 232, 104, 38, 44, 98, 216, 248, 134, 50, 25, 77, 200, 246, 175, 56, 8, 73, 
  161, 226, 216, 97, 31, 59, 88, 215, 244, 159, 53, 6, 74, 169, 229, 214, 109, 33, 45, 80, 
  208, 248, 179, 60, 12, 72, 110, 226, 240, 160, 42, 7, 75, 143, 225, 229, 151, 33, 17, 68, 
  160, 232, 219, 146, 32, 19, 75, 153, 234, 217, 159, 28, 10, 76, 141, 229, 227, 171, 43, 3, 
  78, 104, 230, 245, 183, 68, 9, 67, 74, 205, 242, 194, 138, 19, 27, 71, 142, 235, 228, 182, 
  46, 7, 76, 77, 216, 247, 189, 148, 21, 12, 79, 120, 233, 247, 186, 80, 15, 52, 69, 161, 
  234, 222, 188, 46, 12, 73, 69, 180, 244, 201, 184, 38, 0, 78, 72, 192, 243, 206, 188, 40, 
  5, 72, 73, 171, 236, 223, 185, 59, 9, 59, 76, 147, 241, 239, 179, 101, 18, 22, 79, 109, 
  231, 252, 179, 164, 15, 0, 85, 86, 189, 242, 209, 183, 76, 8, 45, 80, 118, 235, 249, 174, 
  153, 14, 2, 67, 83, 156, 240, 235, 173, 121, 14, 5, 77, 100, 186, 241, 215, 176, 93, 9, 
  18, 74, 112, 195, 246, 208, 179, 83, 11, 24, 72, 112, 200, 246, 212, 174, 89, 12, 12, 75, 
  115, 181, 244, 228, 173, 122, 9, 5, 69, 107, 148, 250, 238, 176, 156, 24, 0, 71, 80, 128, 
  219, 251, 200, 174, 87, 3, 15, 73, 117, 158, 250, 233, 175, 153, 28, 0, 63, 79, 128, 199, 
  250, 215, 176, 127, 11, 4, 65, 95, 128, 223, 251, 197, 169, 110, 6, 7, 70, 103, 133, 229, 
  249, 193, 167, 107, 4, 8, 67, 102, 131, 222, 251, 203, 170, 117, 3, 6, 68, 94, 130, 198, 
  249, 221, 168, 138, 19, 1, 55, 75, 133, 156, 249, 233, 178, 152, 81, 0, 18, 69, 112, 137, 
  214, 252, 205, 169, 135, 14, 2, 46, 79, 129, 149, 249, 239, 184, 158, 104, 0, 10, 59, 97, 
  138, 174, 253, 227, 179, 146, 89, 0, 15, 64, 105, 133, 183, 250, 227, 178, 140, 86, 0, 18, 
  64, 102, 138, 178, 252, 227, 183, 146, 100, 0, 13, 60, 98, 138, 155, 250, 233, 187, 157, 116, 
  11, 5, 51, 73, 135, 140, 212, 251, 213, 174, 125, 70, 0, 23, 62, 111, 141, 165, 253, 228, 
  189, 150, 114, 15, 5, 43, 76, 124, 147, 193, 255, 219, 182, 130, 105, 0, 13, 52, 90, 140, 
  145, 212, 250, 211, 177, 123, 100, 0, 19, 53, 92, 141, 151, 215, 249, 210, 177, 120, 103, 0, 
  14, 56, 84, 140, 150, 188, 255, 215, 182, 139, 108, 10, 6, 43, 71, 136, 151, 162, 255, 221, 
  195, 154, 111, 67, 0, 28, 62, 110, 142, 154, 213, 248, 210, 173, 128, 104, 11, 8, 42, 70, 
  134, 160, 152, 253, 227, 204, 164, 107, 99, 0, 17, 51, 82, 141, 158, 164, 255, 219, 196, 150, 
  107, 90, 0, 24, 59, 89, 139, 159, 164, 255, 219, 202, 149, 111, 91, 0, 22, 51, 83, 143, 
  160, 157, 244, 228, 204, 169, 112, 101, 23, 6, 40, 67, 127, 154, 162, 195, 250, 210, 189, 132, 
  99, 77, 0, 30, 60, 96, 148, 161, 164, 241, 225, 199, 159, 108, 88, 33, 6, 43, 60, 114, 
  150, 162, 169, 255, 215, 193, 149, 106, 90, 11, 13, 45, 68, 125, 152, 166, 174, 254, 212, 191, 
  147, 103, 91, 10, 13, 45, 68, 127, 158, 163, 174, 254, 212, 200, 154, 101, 90, 42, 5, 42, 
  67, 119, 148, 166, 164, 238, 223, 202, 174, 112, 96, 78, 0, 34, 54, 99, 143, 160, 166, 183, 
  250, 207};

const unsigned char fastinvader1[982] = {
  122, 105, 88, 60, 43, 20, 15, 9, 20, 31, 48, 65, 94, 105, 128, 144, 167, 184, 195, 218, 
  224, 235, 240, 246, 252, 255, 252, 252, 252, 246, 246, 240, 235, 224, 218, 207, 201, 195, 184, 178, 
  167, 161, 144, 139, 133, 122, 116, 105, 99, 88, 82, 71, 65, 54, 60, 65, 65, 77, 82, 94, 
  94, 88, 94, 82, 71, 60, 48, 37, 26, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 15, 26, 31, 48, 60, 71, 77, 94, 
  111, 122, 128, 139, 150, 167, 178, 190, 201, 207, 218, 224, 235, 240, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 235, 235, 229, 224, 212, 212, 201, 195, 195, 
  184, 178, 167, 161, 156, 150, 150, 139, 133, 128, 122, 116, 111, 105, 99, 94, 94, 82, 82, 77, 
  77, 65, 65, 60, 43, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 26, 
  43, 60, 77, 82, 99, 105, 116, 116, 128, 128, 128, 128, 128, 128, 122, 116, 111, 105, 99, 105, 
  94, 88, 82, 77, 65, 60, 48, 43, 37, 31, 26, 20, 15, 9, 3, 3, 0, 0, 0, 3, 
  9, 26, 37, 48, 48, 48, 48, 43, 37, 26, 15, 9, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 15, 26, 37, 48, 60, 71, 88, 105, 
  116, 128, 139, 150, 167, 178, 195, 207, 218, 235, 246, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 252, 240, 240, 235, 224, 212, 207, 195, 195, 184, 173, 167, 161, 156, 150, 139, 133, 133, 
  128, 128, 116, 111, 105, 105, 94, 88, 71, 43, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 9, 26, 37, 54, 71, 82, 94, 105, 111, 116, 116, 122, 122, 122, 128, 122, 122, 116, 116, 
  111, 111, 99, 88, 82, 77, 71, 65, 54, 43, 43, 31, 20, 15, 3, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 3, 9, 15, 20, 15, 20, 9, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
  15, 26, 43, 54, 71, 88, 99, 111, 122, 133, 150, 161, 173, 190, 201, 212, 224, 235, 246, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 246, 240, 235, 224, 218, 212, 207, 201, 190, 184, 178, 173, 167, 156, 156, 150, 
  139, 128, 128, 128, 122, 116, 111, 105, 99, 94, 94, 88, 65, 43, 15, 0, 0, 0, 0, 0, 
  0, 0, 0, 3, 15, 31, 48, 65, 82, 94, 105, 116, 128, 133, 139, 144, 150, 150, 144, 150, 
  144, 139, 139, 133, 128, 116, 111, 99, 94, 82, 82, 77, 60, 54, 48, 43, 37, 26, 15, 15, 
  9, 3, 0, 0, 0, 0, 0, 3, 15, 26, 31, 43, 43, 37, 37, 31, 20, 15, 3, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  3, 15, 31, 43, 48, 65, 77, 94, 105, 122, 128, 144, 156, 167, 184, 195, 207, 218, 229, 240, 
  252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 252, 246, 235, 229, 224, 218, 212, 207, 201, 195, 184, 178, 178, 173, 156, 
  150, 150, 139, 128, 128, 128, 122, 116, 111, 99, 94, 88, 88, 82, 77, 71, 71, 65, 60, 54, 
  48, 43, 48, 43, 37, 31, 26, 31, 26, 26, 26, 26, 20, 20, 15, 15, 9, 15, 9, 15, 
  9, 15, 9, 15, 15, 15, 9, 15, 15, 15, 20, 26, 20, 26, 20, 26, 31, 26, 31, 31, 
  31, 31, 37, 37, 43, 43, 43, 48, 54, 48, 54, 60, 54, 60, 60, 65, 71, 65, 71, 77, 
  71, 77, 82, 82, 88, 94, 88, 94, 99, 99, 105, 99, 105, 105, 111, 111, 116, 116, 122, 128, 
  128, 128, 128, 128, 128, 128, 133, 128, 133, 139, 139, 133, 139, 139, 144, 150, 144, 150, 156, 150, 
  156, 150, 156, 161, 156, 161, 156, 161, 161, 161, 161, 167, 161, 167, 173, 167, 173, 173, 173, 167, 
  173, 173, 178, 173, 178, 173, 178, 173, 173, 178, 173, 173, 178, 173, 173, 178, 173, 178, 173, 173, 
  178, 173, 178, 173, 167, 167, 173, 167, 173, 167, 161, 161, 167, 161, 167, 161, 161, 161, 167, 161, 
  156, 161, 161, 161, 156, 150, 156, 156, 156, 156, 150, 156, 156, 156, 150, 144, 144, 150, 144, 150, 
  144, 150, 144, 139, 144, 139, 144, 144, 139, 144, 139, 133, 139, 139, 139, 133, 139, 133, 133, 133, 
  139, 133, 133, 128, 133, 128, 128, 128, 128, 128, 128, 128, 128, 128, 116, 122, 116, 122, 116, 111, 
  116, 116, 111, 111, 116, 111, 116, 111, 116, 111, 111, 116, 111, 116, 111, 111, 116, 111, 116, 111, 
  111, 111};

const unsigned char fastinvader2[1042] = {
  128, 128, 116, 94, 71, 54, 31, 20, 9, 20, 31, 48, 65, 88, 105, 128, 139, 161, 178, 190, 
  207, 218, 229, 240, 252, 252, 252, 255, 255, 252, 252, 252, 240, 240, 229, 224, 212, 207, 201, 184, 
  184, 173, 161, 150, 144, 128, 128, 122, 111, 99, 94, 82, 77, 65, 60, 54, 54, 71, 71, 82, 
  88, 94, 94, 94, 82, 77, 65, 60, 37, 31, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 9, 20, 26, 43, 54, 65, 71, 88, 
  99, 116, 122, 133, 144, 156, 167, 178, 184, 195, 207, 218, 229, 235, 246, 252, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 246, 240, 235, 224, 218, 218, 207, 201, 195, 190, 
  184, 178, 173, 167, 161, 156, 144, 139, 144, 133, 128, 128, 122, 116, 116, 105, 105, 94, 94, 88, 
  82, 77, 77, 65, 65, 60, 60, 54, 54, 48, 43, 43, 37, 37, 31, 31, 26, 26, 15, 3, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 37, 54, 71, 82, 94, 105, 
  111, 122, 128, 133, 133, 139, 133, 139, 139, 139, 133, 139, 128, 128, 128, 116, 116, 111, 111, 99, 
  88, 94, 82, 77, 71, 65, 60, 60, 48, 43, 37, 31, 37, 43, 54, 71, 77, 88, 94, 94, 
  88, 88, 77, 71, 60, 54, 43, 31, 20, 15, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 3, 3, 15, 20, 31, 43, 48, 60, 71, 82, 99, 111, 122, 128, 139, 150, 167, 178, 190, 
  201, 212, 224, 235, 246, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 240, 235, 224, 218, 218, 207, 
  201, 195, 190, 178, 178, 167, 156, 150, 144, 139, 133, 128, 122, 116, 116, 105, 99, 94, 94, 82, 
  71, 77, 71, 60, 54, 54, 48, 43, 37, 31, 26, 26, 26, 20, 15, 3, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 31, 48, 60, 77, 88, 94, 99, 105, 116, 
  116, 122, 122, 128, 122, 122, 116, 116, 111, 105, 99, 94, 88, 82, 77, 65, 60, 54, 48, 48, 
  37, 31, 26, 20, 15, 9, 3, 0, 3, 0, 9, 26, 31, 43, 48, 54, 48, 48, 43, 31, 
  26, 15, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 15, 20, 31, 43, 54, 65, 82, 94, 105, 122, 128, 144, 156, 167, 184, 190, 207, 
  218, 229, 240, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 240, 235, 229, 224, 218, 207, 201, 195, 190, 184, 
  178, 178, 167, 161, 156, 144, 139, 133, 128, 133, 128, 116, 111, 111, 105, 94, 88, 77, 71, 65, 
  60, 60, 54, 54, 48, 48, 43, 43, 37, 31, 26, 20, 3, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 15, 26, 48, 65, 82, 94, 111, 122, 128, 128, 133, 133, 139, 139, 144, 
  139, 144, 139, 139, 133, 128, 128, 116, 116, 111, 99, 94, 88, 82, 82, 71, 60, 54, 48, 43, 
  37, 31, 26, 20, 26, 20, 26, 43, 48, 60, 65, 65, 65, 71, 65, 54, 48, 43, 31, 20, 
  9, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 20, 
  26, 43, 48, 60, 77, 88, 105, 116, 128, 133, 144, 161, 173, 190, 201, 212, 218, 229, 240, 252, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 252, 246, 240, 235, 224, 218, 212, 201, 195, 184, 184, 178, 167, 161, 156, 150, 144, 
  139, 128, 128, 128, 116, 116, 111, 99, 99, 94, 88, 82, 82, 77, 77, 65, 60, 54, 54, 48, 
  43, 43, 37, 31, 31, 26, 20, 26, 20, 15, 15, 20, 15, 15, 15, 15, 9, 9, 9, 9, 
  15, 9, 9, 15, 9, 3, 9, 15, 15, 9, 15, 9, 15, 15, 20, 15, 20, 26, 26, 20, 
  31, 31, 31, 37, 31, 37, 37, 43, 43, 48, 54, 48, 54, 54, 60, 60, 65, 65, 71, 71, 
  77, 77, 82, 82, 88, 88, 94, 99, 94, 99, 105, 105, 111, 116, 111, 116, 116, 122, 122, 128, 
  128, 128, 133, 128, 133, 139, 133, 139, 144, 139, 144, 144, 150, 156, 156, 156, 150, 156, 161, 156, 
  161, 156, 161, 161, 167, 167, 173, 167, 167, 173, 167, 173, 173, 167, 167, 173, 173, 178, 173, 178, 
  173, 178, 178, 173, 178, 178, 173, 178, 173, 178, 178, 173, 173, 178, 173, 178, 184, 178, 184, 178, 
  173, 178, 178, 173, 178, 173, 178, 178, 173, 173, 178, 178, 173, 167, 173, 178, 173, 167, 167, 173, 
  167, 173, 167, 161, 167, 161, 161, 156, 150, 150, 156, 150, 144, 144, 150, 144, 144, 150, 144, 139, 
  139, 144, 139, 144, 139, 144, 139, 133, 133, 133, 139, 133, 139, 133, 128, 133, 133, 128, 128, 133, 
  128, 133, 128, 133, 128, 133, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 122, 
  128, 122, 128, 122, 122, 122, 116, 122, 116, 116, 122, 122, 116, 122, 116, 111, 116, 116, 111, 111, 
  116, 111, 116, 111, 116, 111, 111, 111, 116, 111, 116, 111, 116, 111, 111, 116, 111, 111, 111, 111, 
  111, 116};



// *************************** Capture image dimensions out of BMP**********
#define BUNKERW     ((unsigned char)Bunker0[18])
#define BUNKERH     ((unsigned char)Bunker0[22])
#define ENEMY30W    ((unsigned char)SmallEnemy30PointA[18])
#define ENEMY30H    ((unsigned char)SmallEnemy30PointA[22])
#define ENEMY20W    ((unsigned char)SmallEnemy20PointA[18])
#define ENEMY20H    ((unsigned char)SmallEnemy20PointA[22])
#define ENEMY10W    ((unsigned char)SmallEnemy10PointA[18])
#define ENEMY10H    ((unsigned char)SmallEnemy10PointA[22])
#define ENEMYBONUSW ((unsigned char)SmallEnemyBonus0[18])
#define ENEMYBONUSH ((unsigned char)SmallEnemyBonus0[22])
#define LASERW      ((unsigned char)Laser0[18])
#define LASERH      ((unsigned char)Laser0[22])
#define MISSILEW    ((unsigned char)Missile0[18])
#define MISSILEH    ((unsigned char)Missile0[22])
#define PLAYERW     ((unsigned char)PlayerShip0[18])
#define PLAYERH     ((unsigned char)PlayerShip0[22])
#define START_SCREEN        0
#define GAME_SCREEN         1
#define SHUFFLE_WAIT        30
#define SHUFFLE_WAIT_DEBUG  20
#define ANIMATE_WAIT        25
#define ANIMATE_WAIT_DEBUG  10
#define RANDOM_FORMATIONS   5
#define RANDOM_ENEMY_MOVES  3
#define MAX_LASERS          5
#define MAX_MISSILES        1
#define FULL_ENEMY_LASER_INDEX  10
#define MAX_FIRST_OBJECT    3
#define MAX_SECOND_OBJECT   4
#define MAX_ENEMIES         4
#define OFFSET_COLLISION_ENEMY_X    15
#define OFFSET_COLLISION_ENEMY_Y    8
#define OFFSET_COLLISION_PLAYER_X   16
#define OFFSET_COLLISION_PLAYER_Y   3

// *************************** Enums****************

enum ScreenType{
  START,
  ROUND1,
  TRANSITION_ROUND2,
  ROUND2,
  TRANSITION_ROUND3,
  ROUND3,
  WIN,
  LOSE
}DisplayMode;

enum FireRate{
  SLOW_RATE   = 15,
  MEDIUM_RATE = 10,
  FAST_RATE   = 5
}LaserFire;

enum ProjectileSpeed{
  SLOW_MOVE   = 4,
  MEDIUM_MOVE = 3,
  FAST_MOVE   = 1
}LaserSpeed;

// *************************************************

// *************************** Structures**********
struct imageState {
  unsigned long x;                // x coordinate
  unsigned long y;                // y coordinate
  const unsigned char *image[4];  // ptr->image
  long life;                      // 0=dead, 1=alive
  long height;                    //Offset for object height
  long width;                     //Offset for object width
};          
typedef struct imageState STyp;

struct imageStateB {
  unsigned long x;               // x coordinate
  unsigned long y;               // y coordinate
  const unsigned char *image[4]; // ptr->image
  long life;                     // 0=dead, 1=alive
};          
typedef struct imageStateB BTyp;

struct ProjectileParameters {
  int Speed;      // speed limit of the laser
  int FireRate;   // Speed at which lasers can be generated
  int MaxLasers;  // maximum amount of lasers on screen
  int Count;
  int Index;
};          
typedef struct ProjectileParameters LTyp;
// *************************************************

// **************** Global Variables***********************************************************************************
unsigned long timerCount;
unsigned long semaphore;
unsigned long soundIndex = 0;
const unsigned char *Wave;
unsigned long arrayCount = 0;
unsigned char String[10]; // null-terminated ASCII String1
unsigned long ADCdata;    // 12-bit 0 to 4095 sample
int adc = 0;
//int playerLaserCount   = 0;
//int playerMissileCount = 0;
//int enemyLaserCount    = 0;
//int playerLaserIndex   = 0;
//int playerMissileIndex = 0;
//int enemyLaserIndex    = 0;


const unsigned char *EnemyTypes1[4] = {SmallEnemy10PointA,SmallEnemy20PointA, SmallEnemy30PointA,SmallEnemy20PointA};
const unsigned char *EnemyTypes2[4] = {SmallEnemy10PointB,SmallEnemy20PointB, SmallEnemy30PointB,SmallEnemy20PointB};

// **************** Structure Instances*************

STyp Player[1];
STyp Enemy[MAX_ENEMIES];
STyp LaserImageEnemy[FULL_ENEMY_LASER_INDEX];
STyp LaserImagePlayer[MAX_LASERS];
STyp MissileImagePlayer[MAX_MISSILES];
STyp Bunker[3];
STyp ExplosionObject[4];
//Variables for rules
LTyp LaserParamsPlayer;
LTyp MissileParamsPlayer;
LTyp LaserParamsEnemy;

// *************************************************

// ********************************************************************************************************************

// ******************* Funsction Prototypes *****************************
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void MainLoop1(void);
void InitEnemies(int);
void InitBunkerObjects(enum ScreenType);
void InitPlayer(void);
void InitProjectiles(void);
int firePlayerLaser(int, STyp*, STyp*);
int firePlayerMissile(int, STyp*, STyp*);
void fireLaserEnemy(int, int);
void ClearAllObjects(void);
unsigned long GetRandomNumber(unsigned long);
unsigned long RandomInvaderShuffle(void);
unsigned long RandomInvaderFire(void);
unsigned long FireEnemyLaser(int);
void MoveLeft(void);
void MoveRight(void);
void MoveInvaderShuffle(int);
unsigned long AnimateEnemies(void);
void MovePlayer(void);
int MoveInvaderAdvance(void);
void MoveLaserUp(void);
void MoveLaserDown(void);
void MoveMissile(void);
void DrawEnemies(unsigned long);
void DrawExplosions(int);
void DrawArea(int);
void DrawPlayer(int);
void DrawLasers(void);
void DrawMissiles(void);
void DrawEnemyLasers(void);
enum ScreenType Start_Screen(int);
enum ScreenType LifeCheck(void);
void PlayerLifeDisplay(void);
void Draw(unsigned long, int, int, int, int, int);
int CrashCheck(STyp*, STyp*, STyp*, int, int, int);
void CountLasers(int, int);
void UpdateSettings(void);
void PlaySoundShoot(int);
void PlaySoundInvaderKilled(int);
void PlaySoundExplosion(int);
void PlaySoundHighPitch(int);
void PlaySoundFastInvader1(int);
void UpdateFrame(void);
void Timer2_Init(unsigned long period);
void Timer2A_Start(void);
void Timer2A_Stop(void);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
void DAC_Init(void);
void DAC_Out(unsigned long);
void Sound_Play(const unsigned char*,unsigned long);
void ADC0_Init(void);
unsigned long ADC0_In(void);
void SystickInit(void);
void Buttons_Init(void);
unsigned long Read_Buttons(void);
void LEDs_Init(void);
void LED1_On(void);
void LED1_Off(void);
void LED2_On(void);
void LED2_Off(void);
void Backlight_Init(void);
void Backlight_On(void);
void Backlight_Off(void);

int main(void){

  DisplayMode = START;
  //int semaphoreWait = 0;
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  //srand(time(NULL));
  Nokia5110_Init();
  Timer2_Init(7272);
  DAC_Init();
  LEDs_Init();
  ADC0_Init();
  SystickInit();
	Buttons_Init();
  Backlight_Init();
  EnableInterrupts();
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      // draw buffer
  Backlight_On();
  //Random_Init(4);
  

  while(1)
  {
    while(semaphore==0)
    {  
      //Delay100ms(2);
      //MainLoop1();
    }
    //Update the Frame
    UpdateFrame();
    //Reset the semaphore
    semaphore = 0;
  }
}

//------------Animation Thread section(main thread)------------------------------------------------------------------

// **************InitEnemies*********************************
// Initializes four enemies in start position
// Input: Random number for enemy start position
// Output: none
void InitEnemies(int enemyFormation)
{ 
  int i;
  for(i=0;i<4;i++)                  //Initialise all enemies
  {
    Enemy[i].x = 20*i+enemyFormation;
    Enemy[i].y = 10;
    Enemy[i].image[0] = EnemyTypes1[i];
    Enemy[i].image[1] = EnemyTypes2[i];
    if(EnemyTypes1[i]==SmallEnemy10PointA)
    {
      Enemy[i].height = ENEMY10H;
      Enemy[i].width = ENEMY10W;
    }
    else if(EnemyTypes1[i]==SmallEnemy20PointA)
    {
      Enemy[i].height = ENEMY20H;
      Enemy[i].width = ENEMY20W;
    }
    else if(EnemyTypes1[i]==SmallEnemy30PointA)
    {
      Enemy[i].height = ENEMY30H;
      Enemy[i].width = ENEMY30W;
    }
    else if(EnemyTypes1[i]==SmallEnemy20PointA)
    {
      Enemy[i].height = ENEMY20H;
      Enemy[i].width = ENEMY20W;
    }
    //Enemy[i].image[0] = SmallEnemy30PointA;
    //Enemy[i].image[1] = SmallEnemy30PointB;
    Enemy[i].life = 1;
  }
}

void InitBunkerObjects(enum ScreenType displayMode)
{
  int i = 0;
  switch(displayMode)
  {
    case START:
    {
      for(i=0;i<3;i++)
      {
        Bunker[i].x = 32;
        //Bunker[i].y = 25;
        Bunker[i].y = 47-PLAYERH;
        Bunker[i].height = 5;
        Bunker[i].width = 19;
        Bunker[i].image[0] = Bunker3;
        Bunker[i].image[1] = Bunker2;
        Bunker[i].image[2] = Bunker1;
        Bunker[i].image[3] = Bunker0;
        Bunker[i].life = 3;
        if(i>0)
        {
          Bunker[i].life = 0;
        }
      }
    }
    break;
    case TRANSITION_ROUND2:
    {
      for(i=0;i<3;i++)
      {
        Bunker[i].x = 50;
        Bunker[i].y = 47-PLAYERH;
        Bunker[i].height = 5;
        Bunker[i].width = 19;
        Bunker[i].image[0] = Bunker3;
        Bunker[i].image[1] = Bunker2;
        Bunker[i].image[2] = Bunker1;
        Bunker[i].image[3] = Bunker0;
        Bunker[i].life = 3;
        if(i>0)
        {
          Bunker[i].life = 0;
        }
      }
    }
    break;
    case TRANSITION_ROUND3:
    {
      for(i=0;i<3;i++)
      {
        Bunker[i].x = 5;
        Bunker[i].y = 47-PLAYERH;
        Bunker[i].height = 5;
        Bunker[i].width = 19;
        Bunker[i].image[0] = Bunker3;
        Bunker[i].image[1] = Bunker2;
        Bunker[i].image[2] = Bunker1;
        Bunker[i].image[3] = Bunker0;
        Bunker[i].life = 3;
        if(i>0)
        {
          Bunker[i].life = 0;
        }
      }
    }
    break;
    default:
    {

    }
    break;
  }
}

// **************InitPlayer*********************************
// Initializes player ship in start position
// Input: none
// Output: none
void InitPlayer(void)
{ 
  Player[0].x = 32;
  Player[0].y = 47;
  Player[0].image[0] = PlayerShip0;
  Player[0].image[1] = PlayerShip0;
  Player[0].height = OFFSET_COLLISION_PLAYER_Y;
  Player[0].width = OFFSET_COLLISION_PLAYER_X;
  Player[0].life = 2;
}

// **************InitProjectiles*********************************
// Initializes all Projectile objects and sets all related
// global variables to zero
// Input: none
// Output: none
void InitProjectiles(void)
{ 
  int index = 0;

  for(index=0;index<FULL_ENEMY_LASER_INDEX;index++)
  {
    LaserImageEnemy[index].life = 0;
  }

  for(index=0;index<MAX_LASERS;index++)
  {
    LaserImagePlayer[index].life = 0;
  }

  for(index=0;index<1;index++)
  {
    MissileImagePlayer[index].life = 0;
  }
  LaserParamsPlayer.Count   = 0;
  LaserParamsEnemy.Count    = 0;
  MissileParamsPlayer.Count = 0;
  LaserParamsPlayer.Index   = 0;
  LaserParamsEnemy.Index    = 0;
  MissileParamsPlayer.Index = 0;

}

// **************firePlayerLaser*********************************
// Initializes a laser object at the postion in front 
// of the firing object
// Input: The button code, the Laser object structure, 
//        The firing object structure
// Output: none
int firePlayerLaser(int codeFromButton, STyp *Projectile, STyp *FiringObject)
{
  //static int laserCount = 0;
  static int semaphoreCount_IL = 0;
  int SoundFlag = 0;
  //static int LaserParamsPlayer.Index = 0;
  
  if(LaserParamsPlayer.Index>LaserParamsPlayer.MaxLasers-1)
  {
    LaserParamsPlayer.Index=0;
  }
  if(codeFromButton == 1 && LaserParamsPlayer.Count<LaserParamsPlayer.MaxLasers && semaphoreCount_IL >= LaserParamsPlayer.FireRate)            //Was the button pressed to fire and max amount of lasers not exceeded?
  {
    if(Projectile[LaserParamsPlayer.Index].life==0)
    {
      Projectile[LaserParamsPlayer.Index].x = FiringObject[0].x+8;
      Projectile[LaserParamsPlayer.Index].y = FiringObject[0].y-8;
      Projectile[LaserParamsPlayer.Index].height = LASERH-4;
      Projectile[LaserParamsPlayer.Index].width = LASERW;
      Projectile[LaserParamsPlayer.Index].image[0] = Laser0;
      Projectile[LaserParamsPlayer.Index].image[1] = Laser1;
      Projectile[LaserParamsPlayer.Index].life = 1;
      SoundFlag = 1;
      LaserParamsPlayer.Index++;
      LaserParamsPlayer.Count++;                     //Increase the laser count so that when the button is pressed again a new object is created instead of overwriting
      semaphoreCount_IL=0;
    }
  }
  semaphoreCount_IL++;
  return SoundFlag;
}

// **************firePlayerMissile*********************************
// Initializes a Missile object at the postion in front 
// of the firing object
// Input: The button code, the Laser object structure, 
//        The firing object structure
// Output: none
int firePlayerMissile(int codeFromButton, STyp *Projectile, STyp *FiringObject)
{
  static int semaphoreCount_IL = 0;
  int SoundFlag = 0;
  
  if(MissileParamsPlayer.Index>MissileParamsPlayer.MaxLasers-1)
  {
    MissileParamsPlayer.Index=0;
  }
  if(codeFromButton == 2 && MissileParamsPlayer.Count<MissileParamsPlayer.MaxLasers && semaphoreCount_IL >= MissileParamsPlayer.FireRate)            //Was the button pressed to fire and max amount of lasers not exceeded?
  {
    if(Projectile[MissileParamsPlayer.Index].life==0)
    {
      Projectile[MissileParamsPlayer.Index].x = FiringObject[0].x+8;
      Projectile[MissileParamsPlayer.Index].y = FiringObject[0].y-8;
      Projectile[MissileParamsPlayer.Index].height = MISSILEH-4;
      Projectile[MissileParamsPlayer.Index].width = MISSILEW;
      Projectile[MissileParamsPlayer.Index].image[0] = Missile0;
      Projectile[MissileParamsPlayer.Index].image[1] = Missile1;
      Projectile[MissileParamsPlayer.Index].life = 1;
      SoundFlag = 1;
      MissileParamsPlayer.Index++;
      MissileParamsPlayer.Count++;                     //Increase the laser count so that when the button is pressed again a new object is created instead of overwriting
      semaphoreCount_IL=0;
    }
  }
  semaphoreCount_IL++;
  return SoundFlag;
}

// **************fireLaserEnemy*********************************
// Initializes a laser object at the postion in front 
// of the firing object
// Input: The flag for whether the laser should be fired 
//        the index for which enemy will fire
// Output: none
void fireLaserEnemy(int laserFlag, int enemyIndex)
{
  //static int laserCount = 0;
  static int semaphoreCount_IL = 0;
  //static int LaserParamsEnemy.Index = 0;
  
  if(LaserParamsEnemy.Index>LaserParamsEnemy.MaxLasers-1)
  {
    LaserParamsEnemy.Index=0;
  }
  if(laserFlag == 1 && LaserParamsEnemy.Count<LaserParamsEnemy.MaxLasers && semaphoreCount_IL >= 10)            //Was the button pressed to fire and max amount of lasers not exceeded?
  {
    while(LaserImageEnemy[LaserParamsEnemy.Index].life==1)
    {
      LaserParamsEnemy.Index++;
    }
    LaserImageEnemy[LaserParamsEnemy.Index].x = Enemy[enemyIndex].x+7;
    LaserImageEnemy[LaserParamsEnemy.Index].y = Enemy[enemyIndex].y+7;
    LaserImageEnemy[LaserParamsEnemy.Index].height = LASERH-4;
    LaserImageEnemy[LaserParamsEnemy.Index].width = LASERW;
    LaserImageEnemy[LaserParamsEnemy.Index].image[0] = Laser0;
    LaserImageEnemy[LaserParamsEnemy.Index].image[1] = Laser1;
    LaserImageEnemy[LaserParamsEnemy.Index].life = 1;

    LaserParamsEnemy.Index++;
    LaserParamsEnemy.Count++;                     //Increase the laser count so that when the button is pressed again a new object is created instead of overwriting
    semaphoreCount_IL=0;
  }
  semaphoreCount_IL++;
}

void ClearAllObjects(void)
{
  int index;
  Player[0].life = 0;

  for(index=0;index<FULL_ENEMY_LASER_INDEX;index++)
  {
    LaserImageEnemy[index].life = 0;
  }

  for(index=0;index<MAX_LASERS;index++)
  {
    LaserImagePlayer[index].life=0;
  }

    for(index=0;index<MAX_MISSILES;index++)
  {
    MissileImagePlayer[index].life=0;
  }

  for(index=0;index<3;index++)
  {
    Bunker[index].life=0;
  }
}

unsigned long GetRandomNumber(unsigned long number)
{
  unsigned long randomEnemyFormation = 0;
  randomEnemyFormation = rand()%number;
  return randomEnemyFormation;
}

unsigned long RandomInvaderShuffle(void)
{
  unsigned long shuffleDirection;
  shuffleDirection = GetRandomNumber(RANDOM_ENEMY_MOVES); //Returns a number from 0 to parameter-1

  return shuffleDirection;
}

unsigned long RandomInvaderFire(void)
{
  unsigned long randomEnemyFire;
  int leftOrRight;
  randomEnemyFire = GetRandomNumber(MAX_ENEMIES); //Returns a number from 0 to 3
  while(Enemy[randomEnemyFire].life==0)
  {
    leftOrRight = GetRandomNumber(2); 
    if(leftOrRight==1)
    {
      randomEnemyFire--;
    }
    else
    {
      randomEnemyFire++;
    }
  }
  return randomEnemyFire;
}

unsigned long FireEnemyLaser(int enemyIndex)
{
  unsigned long fireFlag;
  fireFlag = GetRandomNumber(2); //Returns a number from 0 to 1
  if(Enemy[enemyIndex].life==0)
  {
    fireFlag=0;
  }
  return fireFlag;

}

void MoveLeft(void)
{ 
  static int semaphoreCount = 0;
  int i;
  if(semaphoreCount == SHUFFLE_WAIT)
  {
    //Shift all enemies to the left as long as they dont exceed the leftmost limit
    for(i=0;i<4;i++)
    {
      if(Enemy[i].life>0)
      {
        if(Enemy[i].x >= 2)
        {
          Enemy[i].x -= 2;
        } 
        else
        {
          //Enemy[i].life = 0;
          Enemy[0].x = 0;
          Enemy[1].x = 20;
          Enemy[2].x = 40;
          Enemy[3].x = 60;
        } 
      }
    }
    semaphoreCount = 0;
  }
  else
  {
    semaphoreCount++;
  }
}

void MoveRight(void)
{ 
  static int semaphoreCount = 0;
  int i;
  if(semaphoreCount == SHUFFLE_WAIT)
  {
    //Shift all enemies to the right as long as they dont exceed the rightmost limit
    for(i=0;i<4;i++)
    {
      if(Enemy[i].life>0)
      {
        if(Enemy[i].x < 67)
        {
          Enemy[i].x += 2;
        }
        else
        {
          Enemy[0].x = 4;
          Enemy[1].x = 24;
          Enemy[2].x = 44;
          Enemy[3].x = 64;
        }
      }
    }
    semaphoreCount = 0;
  }
  else
  {
    semaphoreCount++;
  }
}

void MoveInvaderShuffle(int shuffleDirection)
{
  if(shuffleDirection == 1)
  {
    MoveRight();
  }
  else if(shuffleDirection == 2)
  {
    MoveLeft();
  }
}

unsigned long AnimateEnemies(void)
{
  static unsigned long frameCount=0;
  static int semaphoreCount = 0;

  if(semaphoreCount == ANIMATE_WAIT)  //Delay time by number of interrupt triggers 
  {
    frameCount = (frameCount+1)&0x01;       //Toggle enemy frame to use
    semaphoreCount = 0;
  }
  else
  {
    semaphoreCount++;
  }
  return frameCount;
}

void MovePlayer(void)
{
  Player[0].x = (float)ADCdata/62.04;
  Player[0].x = (int)Player[0].x;
}

int MoveInvaderAdvance(void)
{
  static int semaphoreWait=0;
  int index=0;
  int soundFlag=0;
  
  if(semaphoreWait>60)
  {
    for(index=0;index<MAX_ENEMIES;index++)
    {
      if(Enemy[index].life!=0)  //All the Invaders that are alive
      {
        if(Enemy[index].y<25)
        {
          Enemy[index].y=Enemy[index].y+1;
          soundFlag=1;
        }
      }
    }
    semaphoreWait=0;
  }
  semaphoreWait++;
  return soundFlag;
}

void MoveLaserUp(void)
{
  static int semaphoreCount = 0;
  int laserIndex;

  if(semaphoreCount == LaserParamsPlayer.Speed)                        //Is it time for laser to move?
  {
    for(laserIndex=0;laserIndex<LaserParamsPlayer.MaxLasers;laserIndex++)    //For all laser objects
    {
      if(LaserImagePlayer[laserIndex].life==1)                               //Is the laser active?
      {
        if(LaserImagePlayer[laserIndex].y == 9)                            //Has the laser reached the end of the screen?
        {
          LaserImagePlayer[laserIndex].life=0;                             //Deactivate laser pbject
          LaserParamsPlayer.Count--;
        }
        LaserImagePlayer[laserIndex].y = LaserImagePlayer[laserIndex].y-1;    //Move the laser up one bit
      }
    }
    semaphoreCount=0;
  }
  semaphoreCount++;
}

void MoveLaserDown(void)
{
  static int semaphoreCount = 0;
  int laserIndex;

  if(semaphoreCount == LaserParamsEnemy.Speed)                        //Is it time for laser to move?
  {
    for(laserIndex=0;laserIndex<FULL_ENEMY_LASER_INDEX;laserIndex++)    //For all laser objects
    {
      if(LaserImageEnemy[laserIndex].life==1)                               //Is the laser active?
      {
        if(LaserImageEnemy[laserIndex].y == 47)                            //Has the laser reached the end of the screen?
        {
          LaserImageEnemy[laserIndex].life=0;                             //Deactivate laser pbject
          LaserParamsEnemy.Count--;
        }
        LaserImageEnemy[laserIndex].y = LaserImageEnemy[laserIndex].y+1;    //Move the laser down one bit
      }
    }
    semaphoreCount=0;
  }
  semaphoreCount++;
}

void MoveMissile(void)
{
  static int semaphoreCount = 0;
  int missileIndex;

  if(semaphoreCount == MissileParamsPlayer.Speed)                        //Is it time for laser to move?
  {
    for(missileIndex=0;missileIndex<MissileParamsPlayer.MaxLasers;missileIndex++)    //For all laser objects
    {
      if(MissileImagePlayer[missileIndex].life==1)                               //Is the laser active?
      {
        if(MissileImagePlayer[missileIndex].y == 47)                            //Has the laser reached the end of the screen?
        {
          MissileImagePlayer[missileIndex].life=0;                             //Deactivate laser pbject
          MissileParamsPlayer.Count--;
        }
        MissileImagePlayer[missileIndex].y = MissileImagePlayer[missileIndex].y-1;    //Move the laser down one bit
      }
    }
    semaphoreCount=0;
  }
  semaphoreCount++;
}


void DrawEnemies(unsigned long enemyFrame)
{
  int enemyIndex;

  for(enemyIndex=0;enemyIndex<MAX_ENEMIES;enemyIndex++)
  {
    if(Enemy[enemyIndex].life > 0)
    {
      Nokia5110_PrintBMP(Enemy[enemyIndex].x, Enemy[enemyIndex].y, Enemy[enemyIndex].image[enemyFrame], 5);
    }
  }
}

void DrawExplosions(int explosionDraw)
{
  static int semaphoreCount = 0;
  if(explosionDraw) //Initial draw when explosion occurs
  {
    Nokia5110_PrintBMP(ExplosionObject[0].x , ExplosionObject[0].y , ExplosionObject[0].image[0], 0);
    semaphoreCount++;
  }
  if(semaphoreCount<10 && semaphoreCount!=0) //Keep the draw for some time after impact
  {
    Nokia5110_PrintBMP(ExplosionObject[0].x , ExplosionObject[0].y , ExplosionObject[0].image[0], 0);
    semaphoreCount++;
  }
  else
  {
    semaphoreCount=0;
  }
}

void DrawArea(int drawBunkerDamage)
{
  int damageState = 0;
  //Check if the Bunker is active/live
  if(Bunker[0].life>0)
  {
    //Read the bunker life count
    damageState = Bunker[0].life;
    Nokia5110_PrintBMP(Bunker[0].x, Bunker[0].y, Bunker[0].image[damageState], 0);
  }
  //if(damageCounter>4)   //Reset the damage counter
  //{
    //damageCounter = 0;
  //}
}

void DrawPlayer(int playerShot)
{
  Nokia5110_PrintBMP(Player[0].x, Player[0].y, Player[0].image[0], 0); // player ship according to ADC value
}

void DrawLasers(void)
{
  int laserIndex;
  for(laserIndex=0;laserIndex<MAX_LASERS;laserIndex++)
  {
    if(LaserImagePlayer[laserIndex].life==1)
    {
      Nokia5110_PrintBMP(LaserImagePlayer[laserIndex].x, LaserImagePlayer[laserIndex].y, LaserImagePlayer[laserIndex].image[0], 0);
    }
  }
}

void DrawMissiles(void)
{
  int laserIndex;
  for(laserIndex=0;laserIndex<MAX_MISSILES;laserIndex++)
  {
    if(MissileImagePlayer[laserIndex].life==1)
    {
      Nokia5110_PrintBMP(MissileImagePlayer[laserIndex].x, MissileImagePlayer[laserIndex].y, MissileImagePlayer[laserIndex].image[0], 0);
    }
  }
}

void DrawEnemyLasers(void)
{
  int laserIndex;
  for(laserIndex=0;laserIndex<10;laserIndex++)
  {
    if(LaserImageEnemy[laserIndex].life==1)
    {
      Nokia5110_PrintBMP(LaserImageEnemy[laserIndex].x, LaserImageEnemy[laserIndex].y, LaserImageEnemy[laserIndex].image[0], 0);
    }
  }
}

int CrashCheck(STyp *ProjectileObject, STyp *TargetObject, STyp *CollisionIndicator, int collideFlag, int maxFirstObject, int maxSecondObject)
{
  //Find the objects we want to compare for collisions
  int firstObjectIndex = 0;
  int secondObjectIndex = 0;
  int crashFlag = 0;
  //Check if the collision objects are live, loop through all of them one type at a time
  for(firstObjectIndex=0;firstObjectIndex<maxFirstObject;firstObjectIndex++)
  {
    //Do something if the first object is live
    if(ProjectileObject[firstObjectIndex].life==1)
    {
      //Loop through second object
      for(secondObjectIndex=0;secondObjectIndex<maxSecondObject;secondObjectIndex++)
      {
        //Check if the second object is live
        if(TargetObject[secondObjectIndex].life>0)
        {
          //Now check collision
          if(ProjectileObject[firstObjectIndex].y-ProjectileObject[firstObjectIndex].height <= TargetObject[secondObjectIndex].y
          && ProjectileObject[firstObjectIndex].y >= TargetObject[secondObjectIndex].y-TargetObject[secondObjectIndex].height
          && ProjectileObject[firstObjectIndex].x+ProjectileObject[firstObjectIndex].width >= TargetObject[secondObjectIndex].x 
          && ProjectileObject[firstObjectIndex].x <= TargetObject[secondObjectIndex].x+TargetObject[secondObjectIndex].width)
          {
            TargetObject[secondObjectIndex].life--;
            ProjectileObject[firstObjectIndex].life=0;
            if(collideFlag==1)
            {
              CollisionIndicator[0].life = 1;
              CollisionIndicator[0].x = TargetObject[secondObjectIndex].x;
              CollisionIndicator[0].y = TargetObject[secondObjectIndex].y;
              CollisionIndicator[0].image[0] = SmallExplosion0;
            }
            crashFlag = 1;
          }   
        }
      }
    }
  }
  return crashFlag;
}

void CountLasers(int laserOrigin, int explodedLaser)
{
  if(laserOrigin==0 && explodedLaser==1)
  {
    LaserParamsPlayer.Count = LaserParamsPlayer.Count-explodedLaser;
  }
  else if(laserOrigin==1 && explodedLaser==1)
  {
    LaserParamsEnemy.Count = LaserParamsEnemy.Count-explodedLaser;
  }
  else if(laserOrigin==2 && explodedLaser==1)
  {
    MissileParamsPlayer.Count = MissileParamsPlayer.Count-explodedLaser;
  }
}

// **************UpdateSettings*********************************
// Make sure the global variables representing the settings are
// in order. Also change the settings if need be
// Input: None
// Output: None
void UpdateSettings(void)
{
  int i;
  int enemyCount = 4;
  LaserParamsEnemy.MaxLasers = 5;
  //Non-Condiitonal settings
  LaserParamsEnemy.Speed = SLOW_MOVE;
  LaserParamsEnemy.FireRate = SLOW_RATE;
  LaserParamsPlayer.Speed = SLOW_MOVE;
  LaserParamsPlayer.FireRate = SLOW_RATE;
  LaserParamsPlayer.MaxLasers = 2;
  MissileParamsPlayer.Speed = MEDIUM_MOVE;
  MissileParamsPlayer.FireRate = SLOW_RATE;
  MissileParamsPlayer.MaxLasers = 1;
  

  //Conditional Settings
  for(i=0;i<MAX_ENEMIES;i++)
  {
    if(Enemy[i].life==0)
    {
      enemyCount--;
    }
  }
  LaserParamsEnemy.MaxLasers = LaserParamsEnemy.MaxLasers-(4-enemyCount);
}

void PlaySoundShoot(int shotsFired)
{
  if(shotsFired)
  {
    Sound_Play(shoot,4080);
  }
}


void PlaySoundInvaderKilled(int invaderKilled)
{
  if(invaderKilled)
  {
    Sound_Play(invaderkilled,3377);
  }
}

void PlaySoundExplosion(int bunkerHit)
{
  if(bunkerHit)
  {
    Sound_Play(explosion,2000);
  }
}

void PlaySoundHighPitch(int soundFlag)
{
  if(soundFlag)
  {
    Sound_Play(highpitch,1802);
  }
}

void PlaySoundFastInvader1(int soundFlag)
{
  if(soundFlag)
  {
    Sound_Play(fastinvader1,982);
  }
}

// **************Start_Screen*********************************
// Clears the screen and initializes all the elements if a 
// button is pressed, otherwise it returns the current screen 
// enum.
// Input: integer input from buttons
// Output: ScreenType enum to show the next screen
enum ScreenType Start_Screen(int codeFromButton)
{
  int randomEnemiesFormation = 0;
  //If any button is pressed, Prepare the round and change the display mode
  if(codeFromButton==1 | codeFromButton==2)
  {
    srand(NVIC_ST_CURRENT_R);
    randomEnemiesFormation = GetRandomNumber(RANDOM_FORMATIONS);
    Nokia5110_Clear();
    ClearAllObjects();
    InitEnemies(randomEnemiesFormation);
    InitBunkerObjects(DisplayMode);
    InitPlayer();
    InitProjectiles();
    DisplayMode = ROUND1;
  }
  else
  {
    DisplayMode = START;
  }
  return DisplayMode;
}

enum ScreenType LifeCheck(void)
{
  int enemyIndex;
  int liveEnemyCount = 0;
  int playerLife = 0;

  //Count the live enemies
  for(enemyIndex=0;enemyIndex<MAX_ENEMIES;enemyIndex++)
  {
    if(Enemy[enemyIndex].life > 0)
    {
      liveEnemyCount=liveEnemyCount+1;
    }
  }

  //Check for player life
  if(Player[0].life > 0)
  {
    playerLife = 1;
  }
  else
  {
    playerLife = 0;
  }

  //Evaluate from results
  if(liveEnemyCount!=0 && playerLife==1)
  {
    DisplayMode = DisplayMode; //Stay in this round
  }
  else if(liveEnemyCount==0 && playerLife==1)
  {
    DisplayMode = DisplayMode++; //Go to next round
  }
  else if(liveEnemyCount!=0 && playerLife==0)
  {
    Sound_Play(highpitch,1802);
    DisplayMode = LOSE; //Go to end round
  }
  return DisplayMode;
}

void PlayerLifeDisplay(void)
{
  if(Player[0].life==2)
  {
    LED1_On();
    LED2_On();
  }
  else if(Player[0].life==1)
  {
    LED1_Off();
    LED2_On();
  }
  else
  {
    LED1_Off();
    LED2_Off();
  }
}

// **************Draw*********************************
// Calls all the functions to draw the different 
// elements, depending on the flags and settings
// Input: none
// Output: none
void Draw(unsigned long enemyFrame, int explosionDraw, int explosionDraw2, int bunkerHit, int bunkerHit1, int playerShot)
{
  Nokia5110_ClearBuffer();
  
  DrawEnemies(enemyFrame);
  DrawExplosions(explosionDraw);
  DrawExplosions(explosionDraw2);
  DrawArea(bunkerHit);
  DrawArea(bunkerHit1);
  DrawPlayer(playerShot);
  DrawLasers();  
  DrawMissiles();
  DrawEnemyLasers();
  Nokia5110_DisplayBuffer();      // draw buffer
}


// **************UpdateFrame*********************************
// Checks which screen mode is active and updates the frame
// Input: none
// Output: none
void UpdateFrame(void)
{
  switch(DisplayMode)
  {
    case START:
    {
      int buttonCode;

      //Was the button pressed?
      buttonCode = Read_Buttons();

      //Setup objects to print
      //Display the start screen
      Nokia5110_PrintBMP(0,48,titlescreennew,7);
      Nokia5110_DisplayBuffer();                  // draw buffer
      //Initialize the Start Screen
      DisplayMode = Start_Screen(buttonCode);
    }
    break;
    case ROUND1:
    {
      int buttonCode, shuffleDirection, enemyIndexFire, enemyFireFlag;
      int lasersCollide, bunkerDamage, bunkerDamage1, enemyHitL, enemyHitM, shootSound, shootSound1, playerHit, enemyAdvanceFlag;
      unsigned long enemyFrame;
      //static int LaserParamsPlayer.Index = 0;

      //Settings for this round
      //Update Laser Settings
      UpdateSettings();

      //Input data and random numbers to work with
      //Random_Init(3);
      buttonCode = Read_Buttons();
      shuffleDirection = RandomInvaderShuffle();
      enemyIndexFire = RandomInvaderFire();            //Which enemy should fire?
      enemyFireFlag  = 0;//FireEnemyLaser(enemyIndexFire);          //Should that chosen enemy fire?
      
      //Create or dissipate Image Objects
      //Move existing objects
      enemyFrame = AnimateEnemies();
      MoveInvaderShuffle(shuffleDirection);
      MovePlayer();
      enemyAdvanceFlag = MoveInvaderAdvance();
      shootSound = firePlayerLaser(buttonCode, LaserImagePlayer, Player);
      shootSound1 = firePlayerMissile(buttonCode, MissileImagePlayer, Player);
      //Fire enemy laser if active
      fireLaserEnemy(enemyFireFlag, enemyIndexFire);
      //Move Lasers
      MoveMissile();      //Move and animate the Missile
      MoveLaserUp();      //Player Laser
      MoveLaserDown();    //Enemy Lasers

      //Collision Detection
      enemyHitL      = CrashCheck(LaserImagePlayer, Enemy, ExplosionObject, 1, MAX_LASERS, MAX_ENEMIES);
      enemyHitM      = CrashCheck(MissileImagePlayer, Enemy, ExplosionObject, 1, MAX_LASERS, MAX_ENEMIES);
      bunkerDamage  = CrashCheck(LaserImagePlayer, Bunker, ExplosionObject, 0, MAX_LASERS, 3);
      bunkerDamage1 = CrashCheck(LaserImageEnemy, Bunker, ExplosionObject, 0, FULL_ENEMY_LASER_INDEX, 3);
      playerHit     = CrashCheck(LaserImageEnemy, Player, ExplosionObject, 1, FULL_ENEMY_LASER_INDEX, MAX_ENEMIES);
      lasersCollide = CrashCheck(LaserImagePlayer, LaserImageEnemy, ExplosionObject, 0, MAX_LASERS, FULL_ENEMY_LASER_INDEX);

      //Adjust the laser object numbers
      CountLasers(0, enemyHitL);
      CountLasers(2, enemyHitM);
      CountLasers(0, bunkerDamage);
      CountLasers(0, lasersCollide);
      CountLasers(1, playerHit);
      CountLasers(1, bunkerDamage1);
      CountLasers(1, lasersCollide);

      //Play Sounds
      PlaySoundShoot(shootSound);
      PlaySoundShoot(shootSound1);
      PlaySoundInvaderKilled(enemyHitL);
      PlaySoundInvaderKilled(enemyHitM);
      PlaySoundExplosion(bunkerDamage1);
      PlaySoundFastInvader1(enemyAdvanceFlag);
      //PlaySoundHighPitch(enemyAdvanceFlag);
      
      //LED Life display
      PlayerLifeDisplay();

      //Print Bitmaps
      Draw(enemyFrame, enemyHitL, enemyHitM, bunkerDamage, bunkerDamage1,playerHit);
      DisplayMode = LifeCheck();
    }
    break;
    case TRANSITION_ROUND2:
    {
      //static int semaphoreCount=0;
      int buttonCode;
      int randomEnemiesFormation = 0;
      //Clear all the objects and variables
      Nokia5110_Clear();
      ClearAllObjects();
      Nokia5110_SetCursor(1, 2);
      Nokia5110_OutString("Round 2");
      //Refresh if the button is pressed
      buttonCode = Read_Buttons();
      if(buttonCode!=0)
      {
        //semaphoreCount = 0;
        randomEnemiesFormation = GetRandomNumber(RANDOM_FORMATIONS);
        InitEnemies(randomEnemiesFormation);
        InitBunkerObjects(DisplayMode);
        InitPlayer();
        InitProjectiles();
        DisplayMode = ROUND2;
      }
    }
    break;
    case ROUND2:
    {
      int buttonCode, shuffleDirection, enemyIndexFire, enemyFireFlag;
      int lasersCollide, bunkerDamage, bunkerDamage1, enemyHitL, enemyHitM, shootSound, playerHit;
      unsigned long enemyFrame;
      //static int LaserParamsPlayer.Index = 0;

      //Settings for this round
      //Update Laser Settings
      UpdateSettings();

      //Input data and random numbers to work with
      //Random_Init(3);
      buttonCode = Read_Buttons();
      shuffleDirection = RandomInvaderShuffle();
      enemyIndexFire = RandomInvaderFire();            //Which enemy should fire?
      enemyFireFlag  = FireEnemyLaser(enemyIndexFire);          //Should that chosen enemy fire?
      
      //Create or dissipate Image Objects
      //Move existing objects
      enemyFrame = AnimateEnemies();
      MoveInvaderShuffle(shuffleDirection);
      MovePlayer();

      shootSound = firePlayerLaser(buttonCode, LaserImagePlayer, Player);
      //Fire enemy laser if active
      fireLaserEnemy(enemyFireFlag, enemyIndexFire);
      
      //Move Lasers
      MoveLaserUp();      //Player Laser
      MoveLaserDown();    //Enemy Lasers

      //Collision Detection
      enemyHitL      = CrashCheck(LaserImagePlayer, Enemy, ExplosionObject, 1, MAX_LASERS, MAX_ENEMIES);
      enemyHitM      = CrashCheck(MissileImagePlayer, Enemy, ExplosionObject, 1, MAX_LASERS, MAX_ENEMIES);
      bunkerDamage  = CrashCheck(LaserImagePlayer, Bunker, ExplosionObject, 0, MAX_LASERS, 3);
      bunkerDamage1 = CrashCheck(LaserImageEnemy, Bunker, ExplosionObject, 0, FULL_ENEMY_LASER_INDEX, 3);
      playerHit     = CrashCheck(LaserImageEnemy, Player, ExplosionObject, 1, FULL_ENEMY_LASER_INDEX, MAX_ENEMIES);
      lasersCollide = CrashCheck(LaserImagePlayer, LaserImageEnemy, ExplosionObject, 0, MAX_LASERS, FULL_ENEMY_LASER_INDEX);

      //Adjust the laser object numbers
      CountLasers(0, enemyHitL);
      CountLasers(2, enemyHitM);
      CountLasers(0, bunkerDamage);
      CountLasers(0, lasersCollide);
      CountLasers(1, playerHit);
      CountLasers(1, bunkerDamage1);
      CountLasers(1, lasersCollide);

      PlaySoundShoot(shootSound);
      PlaySoundInvaderKilled(enemyHitL);
      PlaySoundInvaderKilled(enemyHitM);
      PlaySoundExplosion(bunkerDamage1);
      PlaySoundExplosion(playerHit);
      
      //LED Life
      PlayerLifeDisplay();

      //Print Bitmaps
      Draw(enemyFrame, enemyHitL, enemyHitM, bunkerDamage, bunkerDamage1,playerHit);
      DisplayMode = LifeCheck();
    }
    break;
    case TRANSITION_ROUND3:
    {
      static int semaphoreCount=0;
      int randomEnemiesFormation = 0;
      //Clear all the objects and variables
      Nokia5110_Clear();
      ClearAllObjects();
      Nokia5110_SetCursor(1, 2);
      Nokia5110_OutString("Round 3");
      //Wait for some time then refresh
      if(semaphoreCount>30)
      {
        semaphoreCount = 0;
        randomEnemiesFormation = GetRandomNumber(RANDOM_FORMATIONS);
        InitEnemies(randomEnemiesFormation);
        InitBunkerObjects(DisplayMode);
        InitPlayer();
        InitProjectiles();
        DisplayMode = ROUND3;
        //Nokia5110_SetCursor(2, 2);
        //Nokia5110_OutString("TREd");
      }
      else
      {
        semaphoreCount++;
      }

    }
    break;
    case ROUND3:
    {
      int buttonCode, shuffleDirection, enemyIndexFire, enemyFireFlag;
      int lasersCollide, bunkerDamage, bunkerDamage1, enemyHitL, enemyHitM, shootSound, playerHit;
      unsigned long enemyFrame;
      //static int LaserParamsPlayer.Index = 0;

      //Settings for this round
      //Update Laser Settings
      UpdateSettings();

      //Input data and random numbers to work with
      //Random_Init(3);
      buttonCode = Read_Buttons();
      shuffleDirection = RandomInvaderShuffle();
      enemyIndexFire = RandomInvaderFire();            //Which enemy should fire?
      enemyFireFlag  = FireEnemyLaser(enemyIndexFire);          //Should that chosen enemy fire?
      
      //Create or dissipate Image Objects
      //Move existing objects
      enemyFrame = AnimateEnemies();
      MoveInvaderShuffle(shuffleDirection);
      MovePlayer();
      shootSound = firePlayerLaser(buttonCode, LaserImagePlayer, Player);
      //Fire enemy laser if active
      fireLaserEnemy(enemyFireFlag, enemyIndexFire);
      
      //Move Lasers
      MoveLaserUp();      //Player Laser
      MoveLaserDown();    //Enemy Lasers

      //Collision Detection
      enemyHitL      = CrashCheck(LaserImagePlayer, Enemy, ExplosionObject, 1, MAX_LASERS, MAX_ENEMIES);
      enemyHitM      = CrashCheck(MissileImagePlayer, Enemy, ExplosionObject, 1, MAX_LASERS, MAX_ENEMIES);
      bunkerDamage  = CrashCheck(LaserImagePlayer, Bunker, ExplosionObject, 0, MAX_LASERS, 3);
      bunkerDamage1 = CrashCheck(LaserImageEnemy, Bunker, ExplosionObject, 0, FULL_ENEMY_LASER_INDEX, 3);
      playerHit     = CrashCheck(LaserImageEnemy, Player, ExplosionObject, 1, FULL_ENEMY_LASER_INDEX, MAX_ENEMIES);
      lasersCollide = CrashCheck(LaserImagePlayer, LaserImageEnemy, ExplosionObject, 0, MAX_LASERS, FULL_ENEMY_LASER_INDEX);

      //Adjust the laser object numbers
      CountLasers(0, enemyHitL);
      CountLasers(2, enemyHitM);
      CountLasers(0, bunkerDamage);
      CountLasers(0, lasersCollide);
      CountLasers(1, playerHit);
      CountLasers(1, bunkerDamage1);
      CountLasers(1, lasersCollide);

      PlaySoundShoot(shootSound);
      PlaySoundInvaderKilled(enemyHitL);
      PlaySoundInvaderKilled(enemyHitM);
      PlaySoundExplosion(bunkerDamage1);
      
      //LED Life
      PlayerLifeDisplay();

      //Print Bitmaps
      Draw(enemyFrame, enemyHitL, enemyHitM, bunkerDamage, bunkerDamage1,playerHit);
      DisplayMode = LifeCheck();
    }
    break;
    case WIN:
    {
      int buttonCode;
      buttonCode = Read_Buttons();

      Nokia5110_Clear();
      Nokia5110_SetCursor(1, 1);
      Nokia5110_OutString("Congrats");
      Nokia5110_SetCursor(1, 2);
      Nokia5110_OutString("You Won");
      Nokia5110_SetCursor(2, 4);
      Nokia5110_OutString("Hero");
      //Nokia5110_SetCursor(2, 4);
      //Nokia5110_OutUDec(1234);
            
      if(buttonCode==1 || buttonCode==2)
      {
        DisplayMode = START;
      }
    }
    break;
    case LOSE:
    {
      int buttonCode;
      buttonCode = Read_Buttons();

      Nokia5110_Clear();
      Nokia5110_SetCursor(1, 1);
      Nokia5110_OutString("GAME OVER");
      Nokia5110_SetCursor(2, 2);
      Nokia5110_OutString("You Lost");
      Nokia5110_SetCursor(1, 3);
      Nokia5110_OutString("Try again?");
      //Nokia5110_SetCursor(2, 4);
      //Nokia5110_OutUDec(1234);

      if(buttonCode==1 || buttonCode==2)
      {
        DisplayMode = START;
      }
      
    }
    break;
    default:
    {

    }
    break;
  }
}

// **************Backlight_Init*********************
// Initialize Backlight LED
// Input: none
// Output: none
void Backlight_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x04;    			 //1. Activate clock for PortC
	delay = SYSCTL_RCGC2_R;    			 // allow time for clock to stabilize
	GPIO_PORTC_DIR_R |= 0x80;	 			 //2. Bits PC7 set as output
	GPIO_PORTC_AFSEL_R |= 0x00; 			 //3. No alternate functions
	GPIO_PORTC_DEN_R	|=	0x80;	 			 //4. Set digital enable on pins PB5-4 
	GPIO_PORTC_PCTL_R |= 0x00000000;  //5. configure PB7-0 as GPIO
	GPIO_PORTC_AMSEL_R &= ~0xFF;      //6. Disable analog function
	
}

// **************Backlight_On*********************
// Activate Backlight LED
// Input: none
// Output: none
void Backlight_On(void)
{
		GPIO_PORTC_DATA_R |= 0x80;
}

// **************Backlight_Off*********************
// Deactivate Backlight LED
// Input: none
// Output: none
void Backlight_Off(void)
{
		GPIO_PORTC_DATA_R &= ~0x80;
}

//------------Sound Thread Section------------------------------------------------------------------

// You can use this timer only if you learn how it works
void Timer2_Init(unsigned long period)
{ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  timerCount = 0;
  semaphore = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

void Timer2A_Start(void)
{
  NVIC_EN0_R = 1<<23; // enable IRQ 23 in NVIC
}

void Timer2A_Stop(void)
{
  unsigned long currentData,mask;
  currentData = GPIO_PORTB_DATA_R;
  mask = currentData & 0xF0;
  DAC_Out(mask);
  NVIC_DIS0_R = 1<<23; // disable IRQ 23 in NVIC
}

void Timer2A_Handler(void)
{ 
  unsigned long currentData,mask;
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  if(arrayCount)
  {
    currentData = GPIO_PORTB_DATA_R;
    mask = currentData & 0xF0;
    mask = mask | Wave[soundIndex]>>4;
    DAC_Out(mask);   //Sound.c data is 8 bits therefore use the top 4 bits
    soundIndex = soundIndex + 1;
    arrayCount = arrayCount - 1;
  }
  else
  {
    Timer2A_Stop();
  }
}

void Delay100ms(unsigned long count)
{
  unsigned long volatile time;
  while(count>0)
  {
    time = 727240;  // 0.1sec at 80 MHz
    while(time)
    {
	  	time--;
    }
    count--;
  }
}

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void)
{
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  delay = SYSCTL_RCGC2_R;    						// allow time to finish activating
  GPIO_PORTB_AMSEL_R &= ~0x0F;      		// no analog 
  GPIO_PORTB_PCTL_R &= ~0x0000FFFF; 		// regular function
  GPIO_PORTB_DIR_R |= 0x0F;      				// make PB3-0 out
	GPIO_PORTB_DR8R_R |= 0x0F; 						// set PB3-0 to 8mA drive
  GPIO_PORTB_AFSEL_R &= ~0x0F;   				// disable alt funct on PB3-0
  GPIO_PORTB_DEN_R |= 0x0F;      				// enable digital I/O on PB3-0
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data)
{
	GPIO_PORTB_DATA_R = data;
}

void Sound_Play(const unsigned char *pt, unsigned long count){
  Wave = pt;
  soundIndex = 0;
  arrayCount = count;
  Timer2A_Start();
}

//------------Systick Thread Section------------------------------------------------------------------

// This initialization function sets up the ADC 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source:  channel 1
// SS3 interrupts: enabled but not promoted to controller
void ADC0_Init(void){ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog function on PE2
  SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0 
  delay = SYSCTL_RCGC2_R;         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; // 11) channel Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3
}

//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC0_In(void){  
	unsigned long result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}

// **************SystickInit*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void SystickInit(void)
{
  NVIC_ST_CTRL_R = 0;            // disable SysTick during setup
  //NVIC_ST_RELOAD_R = 0x1312D000;
  NVIC_ST_RELOAD_R = 0x0028B0AA; // set reload value to 2666666, which makes the period = 0.033 which makes an approx 30Hz interrupt trigger
  NVIC_ST_CURRENT_R = 0;      	 // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
}


// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void)
{
  ADCdata = ADC0_In();
  semaphore = 1; // trigger
}

//------------Switches and LED Thread Section------------------------------------------------------------------
// **************Buttons_Init*********************
// Initialize switch key inputs
// Input: none
// Output: none
void Buttons_Init(void){ 
  volatile unsigned long  delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;     // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  //GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port E
  //GPIO_PORTE_CR_R = 0x03;           // allow changes to PE1-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog on PE
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PE7-0
  GPIO_PORTE_DIR_R = 0x00;          // 5) PE7-0 in
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) disable alt funct on PE7-0
  //GPIO_PORTA_PUR_R = 0x03;          // enable pull-up on PE1-0
  GPIO_PORTE_DEN_R = 0x03;          // 7) enable digital I/O on PE1-0
}

// **************Read_Buttons*********************
// Returns Buttons status
// Input: none
// Output: 0-3 depending on buttons pressed
unsigned long Read_Buttons(void)
{
  static int semaphoreCount=0;
  if(semaphoreCount>3)  //Delay for debounce
  {
    semaphoreCount=0;
    return (GPIO_PORTE_DATA_R&0x03); // read the two keys
  }
  else
  {
    semaphoreCount++;
    return 0; // read the two keys
  }
}

// **************LEDs_Init*********************
// Initialize LED's
// Input: none
// Output: none
void LEDs_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x02;    			 //1. Activate clock for PortB
	delay = SYSCTL_RCGC2_R;    			 // allow time for clock to stabilize
	GPIO_PORTB_DIR_R |= 0x30;	 			 //2. Bits PB5-4 set as output
	GPIO_PORTB_AFSEL_R |= 0x00; 			 //3. No alternate functions
	GPIO_PORTB_DEN_R	|=	0x30;	 			 //4. Set digital enable on pins PB5-4 
	GPIO_PORTB_PCTL_R |= 0x00000000;  //5. configure PB7-0 as GPIO
	GPIO_PORTB_AMSEL_R &= ~0xFF;      //6. Disable analog function
	
}

// **************LED1_On*********************
// Activate L1
// Input: none
// Output: none
void LED1_On(void)
{
		GPIO_PORTB_DATA_R |= 0x10;
}

// **************LED2_On*********************
// Activate L2
// Input: none
// Output: none
void LED2_On(void)
{
		GPIO_PORTB_DATA_R |= 0x20;
}

// **************LED1_Off*********************
// Deactivate L1
// Input: none 
// Output: none
void LED1_Off(void)
{
		GPIO_PORTB_DATA_R &= ~0x10;
}

// **************LED2_Off*********************
// Deactivate L2
// Input: none 
// Output: none
void LED2_Off(void)
{
		GPIO_PORTB_DATA_R &= ~0x20;
}

