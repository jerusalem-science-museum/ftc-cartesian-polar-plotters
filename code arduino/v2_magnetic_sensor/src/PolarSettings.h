#pragma once

#define MAX_SPEED 3000
#define ACCELERATION 2000

// MAGNETIC LIMIT SWITCHES
#define LEFT_LIMIT_SW_PIN 10
#define RIGHT_LIMIT_SW_PIN 11

// steps per click from encoder.
// Scaling factor between encoders to accelstepper movement.
#define POLAR_STEPPER_STEPSIZE 2

// (0,0) is (top) left motor location, in mm.
// x positive direction is right. 
// y positive direction is down.
#define POLAR_Y_MIN_LIMIT 160
#define POLAR_X_MIN_LIMIT 230
#define POLAR_Y_MAX_LIMIT 720
#define POLAR_X_MAX_LIMIT 670

// extra legal area for getting to the end of the maze (outside of the normal legal area).
#define POLAR_X_END_OF_MAZE_MIN 450
#define POLAR_X_END_OF_MAZE_MAX 530
#define POLAR_Y_END_OF_MAZE 770

// 1000 is 8cm from a simple test done.
#define STEPS_PER_MM (12.5)

#define MOTORS_DISTANCE (900)  // MM distance between the motors

// calibration (metal detectors) location in mm from (top) left motor. 
// y positive direction is down.
#define X_HOME 450
#define Y_HOME 370

#define X_MAZE_START 410
#define Y_MAZE_START 226