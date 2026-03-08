#include <Arduino.h>

#define ENCODER_DEBUG 0

const float MAX_ANGLE = 360.0f;

class IEncoder {
public:
  virtual int readDelta() = 0;
  virtual bool is_pressed() = 0;
  virtual ~IEncoder() {}
};

class RotaryEncoder : public IEncoder
{
private:
  const int out_pin_;

  bool initialized_ = false;

  // Zéro défini au 1er appel
  float zero_angle_deg_ = 0.0f;

  // Dernier angle relatif [0..360)
  float last_rel_angle_deg_ = 0.0f;

  // Accumulation signée de rotation (en degrés)
  float accum_deg_ = 0.0f;

  // 360 = un "clic" par tour complet
  static constexpr float ANGLE_PER_CLICK = 8.0f;

  static constexpr int adcMax()
  {
  #if defined(ARDUINO_ARCH_ESP32)
    return 4095;
  #else
    return 1023;  // UNO/Nano/Mega
  #endif
  }

  // Lit l'angle absolu [0..360)
  float readAngleAbsDeg() const
  {
    int raw = analogRead(out_pin_);
    raw = constrain(raw, 0, adcMax());

    // conversion ADC -> angle
    float angle = (raw * MAX_ANGLE) / float(adcMax() + 1);  // [0..360)
    if (angle >= MAX_ANGLE) angle -= MAX_ANGLE;
    return angle;
  }

  // Normalise en [0..360)
  static float normalize360(float a)
  {
    while (a < 0.0f)   a += MAX_ANGLE;
    while (a >= MAX_ANGLE) a -= MAX_ANGLE;
    return a;
  }

  // Différence signée minimale entre 2 angles (gère 359 -> 0)
  static float wrapDiff(float now_deg, float prev_deg)
  {
    float diff = now_deg - prev_deg;

    if (diff >  (MAX_ANGLE * 0.5f)) diff -= MAX_ANGLE;
    if (diff < -(MAX_ANGLE * 0.5f)) diff += MAX_ANGLE;

    return diff;
  }

public:
  RotaryEncoder(int out_pin)
  : out_pin_(out_pin)
  {
    pinMode(out_pin_, INPUT);
  }

  int readDelta() override
  {
    // 1) Lire angle absolu
    float abs_angle = readAngleAbsDeg();

    // 2) Premier appel = définir le zéro
    if (!initialized_)
    {
      zero_angle_deg_ = abs_angle;     // référence
      last_rel_angle_deg_ = 0.0f;      // angle relatif initial = 0
      accum_deg_ = 0.0f;
      initialized_ = true;

      #if ENCODER_DEBUG
      Serial.print("Zero set at abs=");
      Serial.println(zero_angle_deg_, 2);
      #endif

      return 0;
    }

    // 3) Angle relatif normalisé [0..360)
    float rel_angle = normalize360(abs_angle - zero_angle_deg_);

    // 4) Calcul variation signée depuis la dernière lecture
    float diff = wrapDiff(rel_angle, last_rel_angle_deg_);

    // Mettre à jour le dernier angle relatif
    last_rel_angle_deg_ = rel_angle;

    // 5) Filtre anti-jitter (bruit)
    if (fabs(diff) < 0.2f)
      return 0;

    // 6) Accumuler la rotation signée
    accum_deg_ += diff;

    #if ENCODER_DEBUG
    Serial.print("abs=");
    Serial.print(abs_angle, 2);
    Serial.print(" rel=");
    Serial.print(rel_angle, 2);
    Serial.print(" diff=");
    Serial.print(diff, 2);
    Serial.print(" accum=");
    Serial.println(accum_deg_, 2);
    #endif

    // 7) Si on a atteint 1 tour (360°), renvoyer +4 ou -4 selon le sens
    if (accum_deg_ >= ANGLE_PER_CLICK)
    {
      accum_deg_ -= ANGLE_PER_CLICK;   // garder le reste
      return +4;
    }

    if (accum_deg_ <= -ANGLE_PER_CLICK)
    {
      accum_deg_ += ANGLE_PER_CLICK;   // garder le reste
      return -4;
    }

    return 0;
  }

  bool is_pressed() override
  {
    return false;
  }

  // Optionnel : utile pour debug
  float getRelativeAngleDeg() const
  {
    if (!initialized_) return 0.0f;
    float abs_angle = readAngleAbsDeg();
    return normalize360(abs_angle - zero_angle_deg_);
  }
};


// =====================
// Exemple d’utilisation
// =====================
RotaryEncoder encoder(A7);   // ou A0 selon ta carte
long position = 0;

void setup()
{
  Serial.begin(115200);
  delay(200);
  Serial.println("AS5600 encoder test (zero at first read)");
}

void loop()
{
  int d = encoder.readDelta();

  if (d != 0)
  {
    position += d;
    Serial.print("delta=");
    Serial.print(d);
    Serial.print("  position=");
    Serial.println(position);
  }

  // Debug angle relatif (optionnel)
  // Serial.println(encoder.getRelativeAngleDeg());

 //Serial.println(analogRead(A7));

  delay(5);
}