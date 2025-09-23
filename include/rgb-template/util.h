#pragma once
#include "vex.h"
#include <string>

// Reduces an angle to the range [0, 360).
float normalize360(float angle);

// Reduces an angle to the range [-180, 180).
float normalize180(float angle);

// Clamps a value between a minimum and maximum value.
float threshold(float input, float min, float max);

// Converts a percentage to a voltage.
float toVolt(float percent);

// Creates a dead zone for a joystick axis.
float deadband(float input, float width);

// Checks if all motors are connected and not overheating.
bool checkMotors(int motorCount, int temperatureLimit = 50);

// Prints a message to the controller screen with right-padding to 20 characters.
void printControllerScreen(const char* message);


