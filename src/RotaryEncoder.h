#pragma once

#include <Arduino.h>
#include "IEncoder.h"
#include "Settings.h"

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


class RotaryEncoder : public IEncoder
{
  private:
    const int bit_0_pin_;
    const int bit_1_pin_;
    const int push_button_pin_;
    byte prev_encoder_read_;
    byte new_encoder_read_;
    int turn_direction_sum = 0;
  public:  
  int readDelta()
    {
      prev_encoder_read_ = new_encoder_read_ ;
      new_encoder_read_ = (((digitalRead(bit_1_pin_)) << 1) + (digitalRead(bit_0_pin_))) ;
      byte check_direction  = (prev_encoder_read_ << 2) + new_encoder_read_  ; // x4 = 2 rotate left 

      switch (check_direction) // see https://commons.wikimedia.org/wiki/File:Incremental_directional_encoder.gif
      {
        case 1: case 7: case 8: case 14:
          turn_direction_sum--;
          break;
        case 2: case 4: case 11: case 13:
          turn_direction_sum++;
          break;
        default:
          return 0;
      }
      #if ENCODER_DEBUG
      Serial.println(check_direction);
      Serial.println(turn_direction_sum);
      #endif
      if(abs(turn_direction_sum) == 4)
      {
        int res = turn_direction_sum;
        turn_direction_sum = 0;
        return res;
      }
      return 0;
    }

    RotaryEncoder(int bit_0_pin,int bit_1_pin,int push_button_pin)
    :bit_0_pin_(bit_0_pin),bit_1_pin_(bit_1_pin),push_button_pin_(push_button_pin),
    prev_encoder_read_(0), new_encoder_read_(0)
    {
      pinMode(bit_0_pin_, INPUT_PULLUP);  
      pinMode(bit_1_pin_, INPUT_PULLUP);  
      pinMode(push_button_pin_, INPUT_PULLUP);
    }

    bool is_pressed() override{
      return !digitalRead(push_button_pin_);
    }


};