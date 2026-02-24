// 
/*==========Arduino Nano pinout======
 * IO map see J:\Amir Design\PCB_Design\EASYEDA JLBPCB\projects\SHIELD_V4\Shield_Amir_Shalev_V_0_0
 * Encoder A - Left, B-right
 *                                    _______
 *                               TXD-|       |-Vin
 *                               RXD-|       |-Gnd (Display)
 *                               RST-|       |-RST
 *                To Encoders    GND-|       |-+5V (Display)
 *                  DIR_1_PIN     D2-|       |-A7
 *                  DIR_2_PIN     D3-|       |-A6
 *                                D4-|       |-A5 SCL (Display)
 *                 STEP_1_PIN     D5-|       |-A4 SDA (Display)
 *                 STEP_2_PIN     D6-|       |-A3 ENCODER_B_BUTTON
 *                     UV_PIN     D7-|       |-A2 ENCODER_A_BUTTON
 *                     EN_PIN     D8-|       |-A1 ENCODER_B_BIT_1
 *             ENCODER_A_BIT_0    D9-|       |-A0 ENCODER_A_BIT_1
 *          LEFT_LIMIT_SW_PIN*   D10-|       |-Ref
 *         RIGHT_LIMIT_SW_PIN*   D11-|       |-3.3V
 *             ENCODER_B_BIT_0   D12-|       |-D13
 *                                    --USB--
 *
 * * Polar mode only (defined in PolarSettings.h)
 * ! Nano can use only D2, D3 as interrupt - encoder B must read by polling
 * Mechanical encoder like https://www.aliexpress.com/item/1005005239756119.html
 */ 

#pragma once

#define USE_POLAR_MODE true

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

//UV LED CONFIGURATIONS
#define UV_PIN 7
#define UV_AUTO_TURN_OFF_TIME 3000 // ms
#define GO_TO_START_OF_MAZE 60000 // ms

struct Point { int x, y; };
