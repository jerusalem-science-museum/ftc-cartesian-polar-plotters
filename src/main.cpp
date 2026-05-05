#include <Arduino.h>
#include <AccelStepper.h>
#include "MagneticEncoder.h"
#include "IMode.h"
#include "PolarMode.h"
#include "CartesianMode.h"
#include "Settings.h"
#include "PlotterSystem.h"


AccelStepper stepper_1(AccelStepper::DRIVER, STEP_1_PIN, DIR_1_PIN);
AccelStepper stepper_2(AccelStepper::DRIVER, STEP_2_PIN, DIR_2_PIN);
#if USE_POLAR_MODE
PolarMode mode = PolarMode(&stepper_1, &stepper_2);
#else
CartesianMode mode = CartesianMode(&stepper_1, &stepper_2);
#endif

MagneticEncoder encoder_1 = MagneticEncoder(MAGNETIC_SENS_A);
MagneticEncoder encoder_2 = MagneticEncoder(MAGNETIC_SENS_B);

PlotterSystem p = PlotterSystem(&encoder_1, &encoder_2, &mode);

void setup()
{
    Serial.begin(115200);
    /** Init Joystick input pins **/
    /** AUTO HOME**/
    p.calibrate();       
}


void loop()
{
    
    p.loop();
    
}
