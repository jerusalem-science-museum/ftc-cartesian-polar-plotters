#pragma once

#include <Arduino.h>
#include "IEncoder.h"
#include "Settings.h"

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

class MagneticEncoder : public IEncoder
{
  private:
    const int out_pin_;   // ==> AS5600 OUT (analog pin)

    float zero_angle_deg_ = 0.0f;
    float last_angle_deg_ = 0.0f;
    float turn_accum_deg_ = 0.0f;
    bool initialized_ = false;

    static constexpr float MAX_ANGLE_DEG = 360.0f;

    // Sensibilité : plus petit = plus sensible
    static constexpr float AS5600_ANGLE_PER_CLICK_DEG = 10.0f;

    static constexpr int adcMax()
    {
    #if defined(ARDUINO_ARCH_ESP32)
      return 4095;
    #else
      return 1023; // UNO/Nano/Mega
    #endif
    }

    float readAngleAbsDeg_() const
    {
      int raw = analogRead(out_pin_);
      raw = constrain(raw, 0, adcMax());

      float angle = (raw * MAX_ANGLE_DEG) / float(adcMax() + 1);
      if (angle >= MAX_ANGLE_DEG) angle -= MAX_ANGLE_DEG;
      return angle;
    }

    static float normalize360_(float a)
    {
      while (a < 0.0f) a += MAX_ANGLE_DEG;
      while (a >= MAX_ANGLE_DEG) a -= MAX_ANGLE_DEG;
      return a;
    }

    static float wrapDiff_(float now, float prev)
    {
      float diff = now - prev;
      if (diff >  (MAX_ANGLE_DEG / 2.0f)) diff -= MAX_ANGLE_DEG;
      if (diff < -(MAX_ANGLE_DEG / 2.0f)) diff += MAX_ANGLE_DEG;
      return diff;
    }

  public:

    // Même logique qu'avant : retourne +4 / -4 / 0
    int readDelta() override
    {
      float abs_angle = readAngleAbsDeg_();

      // Premier appel => angle actuel devient le zéro
      if (!initialized_)
      {
        zero_angle_deg_ = abs_angle;
        last_angle_deg_ = 0.0f;
        turn_accum_deg_ = 0.0f;
        initialized_ = true;

        #if ENCODER_DEBUG
        Serial.print("AS5600 zero set to ");
        Serial.println(zero_angle_deg_, 2);
        #endif

        return 0;
      }

      float rel_angle = normalize360_(abs_angle - zero_angle_deg_);
      float diff = wrapDiff_(rel_angle, last_angle_deg_);
      last_angle_deg_ = rel_angle;

      // filtre anti-jitter
      if (fabs(diff) < 0.2f) return 0;

      turn_accum_deg_ += diff;

      if (turn_accum_deg_ >= AS5600_ANGLE_PER_CLICK_DEG)
      {
        turn_accum_deg_ -= AS5600_ANGLE_PER_CLICK_DEG;
        return +4;
      }
      if (turn_accum_deg_ <= -AS5600_ANGLE_PER_CLICK_DEG)
      {
        turn_accum_deg_ += AS5600_ANGLE_PER_CLICK_DEG;
        return -4;
      }

      return 0;
    }

    // Constructeur simplifié : seulement OUT
    RotaryEncoder(int pin_out)
    : out_pin_(pin_out)
    {
      pinMode(out_pin_, INPUT);
    }

    bool is_pressed() override
    {
      return false; // pas de bouton dans cette version
    }
};
