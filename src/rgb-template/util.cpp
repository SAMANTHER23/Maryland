#include "vex.h"

float normalize180(float angle) {
  return fmod(angle + 540, 360) - 180;
}

float normalize360(float angle) {
  return fmod(angle + 360, 360);
}

float threshold(float input, float min, float max){
  if(input > max) return max;
  if(input < min) return min;
  return input;
}

float percentToVolt(float percent){
  return percent*12.0/100.0;
}

float deadband(float input, float width){
  if (fabs(input)<width){
    return 0;
  }
  return input;
}

double curveFunction(double x, double curveScale) {
  return (powf(2.718, -(curveScale / 10)) + powf(2.718, (fabs(x) - 100) / 10) * (1 - powf(2.718, -(curveScale / 10)))) * x;
}

bool checkMotors(int motorCount, int temperatureLimit) {
  int count = 0;
  int t = 0;
  for (int i = 0; i < 20; i++) {
    motor m = motor(i);
    if (m.installed()) {
      count++;
      t = m.temperature(celsius);
      if (t > temperatureLimit) {
        controller(primary).Screen.print("motor %d is %dC           ", i + 1, t);
        controller(primary).rumble("---");
        return false;
      }
    }
  }
  if (count < motorCount) {
    controller(primary).Screen.print("%d motor is disconnected      ", motorCount - count);
    controller(primary).rumble("---");
    return false;
  }
  return true;
}

void printControllerScreen(const char* message) {
  char padded[25];
  snprintf(padded, sizeof(padded), "%-24s", message);
  controller(primary).Screen.print("%s", padded);
}


