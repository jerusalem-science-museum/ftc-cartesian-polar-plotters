#include <Wire.h>
#include <AS5600.h>
#define ONLY_OUT 1
AS5600 as5600;

void setup() {
  Serial.begin(115200);
  delay(200);

  // ===== I2C START =====
  // UNO / Nano / Mega : pas besoin de préciser les pins
  Wire.begin();

  // IMPORTANT (lib RobTillaart): appeler Wire.begin() avant as5600.begin()
  // La lib le demande depuis les versions récentes.
  #ifndef ONLY_OUT
  if (!as5600.begin()) {
    Serial.println("AS5600 non detecte !");
    while (1);
  }
  #endif

  Serial.println("AS5600 detecte.");
  // Exemple: on force OUT en analog 100%
  as5600.setOutputMode(AS5600_OUTMODE_ANALOG_100);
  //uint16_t z = as5600.rawAngle();
  //as5600.setZPosition(z);
  //as5600.setZPosition(4095);
  //delay(2);
  //as5600.setMPosition(4095);
  //as5600.setMaxAngle(4095);
  Serial.println(as5600.getZPosition());
  Serial.println(as5600.getMPosition());
  Serial.println(as5600.getMaxAngle());


  show_angle_out();
  show_angle_i2c();

  
}

void loop() {
  
  show_angle_out();
  show_angle_i2c();
  Serial.println();
  delay(100);
}

void show_angle_i2c(){
   // Angle brut 12 bits (0..4095)
  uint16_t read_i2c = as5600.readAngle();
  uint16_t raw_i2c = as5600.rawAngle();


  // Conversion degrés
  float deg_i2c = (raw_i2c * 360.0) / 4096.0;
  float deg_i2c_read = (read_i2c * 360.0) / 4096.0;

  Serial.print("Read from i2c: ");
  Serial.print(read_i2c);
  Serial.print("  |  Deg from i2c: ");
  Serial.println(deg_i2c_read, 2);
  

  Serial.print("Raw from i2c: ");
  Serial.print(raw_i2c);
  Serial.print("  |  Deg from i2c: ");
  Serial.println(deg_i2c, 2);

 

}

void show_angle_out(){
  int raw = analogRead(A0);
  float deg = (raw * 360.0) / 1024.0;

  Serial.print("Read from out: ");
  Serial.print(raw);
  Serial.print("  |  Deg from out: ");
  Serial.println(deg, 2);
}