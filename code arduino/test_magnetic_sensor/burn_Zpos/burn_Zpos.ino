#include <Wire.h>
#include <AS5600.h>


#define BUTTON_IO 7
#define LED_BURN_STATUS 8
#define LED_MAGNET_STATUS 9
#define OUT_PIN A0

bool check = false;
bool first_press = false;
const int BOUNCE_TIME = 50;
bool PRESS_BUTTON() {
  // Check if the button is pressed
  if (digitalRead(BUTTON_IO) == LOW && check == LOW) {
     //Serial.println("press :");
     check = HIGH;         // Mark that the button is being pressed
    delay(BOUNCE_TIME); // Apply debounce delay
  }

  // Check if the button is released
  if (digitalRead(BUTTON_IO) == HIGH && check == HIGH) {
    //Serial.println("unpress");
    check = LOW;  // Reset the state for the next button press
    return HIGH;  // Indicate that the button was successfully pressed and released
  }
  return LOW; // Return false if the button is not in the desired state
}
AS5600 as5600;
static const uint8_t AS5600_ADDR = 0x36;

uint8_t read8(uint8_t reg) {
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(AS5600_ADDR, (uint8_t)1);
  return Wire.read();
}

uint16_t read16(uint8_t regMSB) {
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(regMSB);
  Wire.endTransmission(false);
  Wire.requestFrom(AS5600_ADDR, (uint8_t)2);
  uint16_t msb = Wire.read();
  uint16_t lsb = Wire.read();
  return (msb << 8) | lsb;
}

void write8(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
  delay(2); // > 1 ms
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Wire.begin();
  pinMode(BUTTON_IO  ,INPUT_PULLUP);
  pinMode(LED_BURN_STATUS  ,OUTPUT);
  pinMode(LED_MAGNET_STATUS,OUTPUT);
  pinMode(OUT_PIN,OUTPUT);
  
  digitalWrite(LED_MAGNET_STATUS,LOW);
  digitalWrite(LED_BURN_STATUS,LOW);
  digitalWrite(OUT_PIN,LOW);

  delay(100);
  pinMode(OUT_PIN,INPUT);

  if (!as5600.begin()) {
    Serial.println("AS5600 non detecte !");
     digitalWrite(LED_MAGNET_STATUS,HIGH);
    while (1);
  }
  Serial.println("AS5600 detecte.");

  // OUT analog 100% (optionnel)
  as5600.setOutputMode(AS5600_OUTMODE_ANALOG_100);

  // 1) Vérifier combien de burns déjà faits (ZMCO)
  uint8_t zmco = read8(0x00); // ZMCO :contentReference[oaicite:3]{index=3}
  Serial.print("ZMCO = 0x"); Serial.println(zmco, HEX);
  // (si ZMCO est déjà au max, ne pas continuer)

  // 2) Vérifier aimant détecté (MD dans STATUS=0x0B bit5)
  uint8_t status = read8(0x0B);
  bool md = (status & (1 << 5)) != 0;
  Serial.print("STATUS=0x"); Serial.print(status, HEX);
  Serial.print("  MD="); Serial.println(md ? "1" : "0");
  if (!md) {
    Serial.println("STOP: aimant non detecte (MD=0). Ne pas bruler !");
    while (1);
  }

  // 3) Mettre l’aimant EXACTEMENT sur la position que tu veux comme zero
  delay(500); // laisse-toi une demi-seconde si tu bouges l’aimant

  // 4) Lire RAW ANGLE (0x0C/0x0D) puis setZPosition = RAW
  uint16_t raw = read16(0x0C) & 0x0FFF;
  Serial.print("RAW = "); Serial.println(raw);

  as5600.setZPosition(raw);   // écrit 0x01/0x02 (ZPOS) :contentReference[oaicite:4]{index=4}
  //as5600.setZPosition(0);   // écrit 0x01/0x02 (ZPOS) :contentReference[oaicite:4]{index=4}
  delay(2);

  Serial.print("ZPOS lu via lib = ");
  Serial.println(as5600.getZPosition());

  // 5) BURN_ANGLE : écrire 0x80 dans 0xFF :contentReference[oaicite:5]{index=5}
  Serial.println("ATTENTION: BURN_ANGLE (OTP) va etre execute !");
  delay(1000); // dernière seconde de sécurité

 
}

void loop() {

  bool button_pressed = PRESS_BUTTON();
  //bool button_pressed = true;

  if(button_pressed && first_press == false){
     write8(0xFF, 0x80); // Burn_Angle :contentReference[oaicite:6]{index=6}
     Serial.println("BURN_ANGLE envoye (0xFF <- 0x80).");
     Serial.println("Maintenant coupe/rallume le capteur, puis relis ZPOS.");
     first_press = true;
     digitalWrite(LED_BURN_STATUS,HIGH);
  }










}