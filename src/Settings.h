// 
/*==========Arduino Nano pinout====== 
 * IO map see J:\Amir Design\PCB_Design\EASYEDA JLBPCB\projects\SHIELD_V4\Shield_Amir_Shalev_V_0_0
 * Encoder A - Left, B-right 
 *                      _______
 *                 TXD-|       |-Vin 
 *                 RXD-|       |-Gnd 
 *                 RST-|       |-RST
 *                 GND-|       |-+5V 
 *       DIR_1_PIN  D2-|       |-A7 MAGNETIC_SENS_A
 *       DIR_2_PIN  D3-|       |-A6 MAGNETIC_SENS_B
 *                  D4-|       |-A5 
 *      STEP_1_PIN  D5-|       |-A4 
 *      STEP_2_PIN  D6-|       |-A3 
 *          UV_PIN  D7-|       |-A2 
 *          EN_PIN  D8-|       |-A1 
 *  Encoder A bit 1 D9-|       |-A0 t 
 *                 D10-|       |-Ref
 *                 D11-|       |-3.3V   
 * Encoder B bit 1 D12-|       |-D13
 *                      --USB--          
 *                     
 * ! Nano can use only D2, D3 as interupt -  encoder B must read by pulling                      
 * Mechanical encoder like https://www.aliexpress.com/item/1005005239756119.html 
 */ 

#pragma once

#define USE_POLAR_MODE false

#define ENABLE_SOFT_LIMIT true

#define DEBUG_MODE false
#define PRODUCTION_MODE true // disables buttons
#define ENCODER_DEBUG false

// STEPPERS CONFIGURATIONS
#define STEP_1_PIN 5
#define STEP_2_PIN 6
#define DIR_1_PIN 2
#define DIR_2_PIN 3
#define EN_PIN 8 // common enable pin for both motors.

// ENCODERS CONFIGURATIONS
#define ENCODER_A_BIT_1 (A0) // input IO for gray code bit 0 
#define ENCODER_A_BIT_0 (9) // input IO for gray code bit 1
#define ENCODER_B_BIT_1 (A1) // input IO for gray code bit 0
#define ENCODER_B_BIT_0 (12) // input IO for gray code bit 1
#define ENCODER_A_BUTTON (A2)
#define ENCODER_B_BUTTON (A3)
#define ENCODER_READ_INTERVAL 200 //microseconds
// MAGNETIC ENCODER CONFIGURATIONS
#define MAGNETIC_SENS_B (A6) // input IO for gray code bit 0 
#define MAGNETIC_SENS_A (A7) // input IO for gray code bit 0

//UV LED CONFIGURATIONS
#define UV_PIN 7
#define UV_AUTO_TURN_OFF_TIME 3000 // ms
#define GO_TO_START_OF_MAZE 60000 // ms

struct Point { int x, y; };
