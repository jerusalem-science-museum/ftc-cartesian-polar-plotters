#pragma once

#include <Arduino.h>
#include "IEncoder.h"
#include "Settings.h"

class RotaryEncoder : public IEncoder
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

      #if ENCODER_DEBUG
      Serial.print("abs=");
      Serial.print(abs_angle, 2);
      Serial.print(" rel=");
      Serial.print(rel_angle, 2);
      Serial.print(" diff=");
      Serial.print(diff, 2);
      Serial.print(" accum=");
      Serial.println(turn_accum_deg_, 2);
      #endif

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