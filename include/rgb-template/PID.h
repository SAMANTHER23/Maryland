#pragma once
#include "vex.h"

// A class to represent a PID controller.
class PID
{

private: 
  // The error between the target and the current value.
  float error = 0;
  // The proportional gain.
  float kp = 0;
  // The integral gain.
  float ki = 0;
  // The derivative gain.
  float kd = 0;
  // The error at which to start integrating.
  float starti = 0;
  // The error at which the PID is considered settled.
  float settleError = 0;
  // The time the PID must be settled for to be considered done.
  float settleTime = 0;
  // The maximum time the PID can run for.
  float timeout = 0;
  // The accumulated error for the integral term.
  float accumulatedError = 0;
  // The error from the previous iteration.
  float previousError = 0;
  // The output of the PID controller.
  float output = 0;
  // The time the PID has been settled for.
  float timeSpentSettled = 0;
  // The time the PID has been running for.
  float timeSpentRunning = 0;
  
public:
  // A constructor for a simple PID controller with only P and D terms.
  PID(float error, float kp, float kd);
  // A constructor for a full PID controller with P, I, and D terms, as well as exit conditions.
  PID(float error, float kp, float ki, float kd, float starti, float settleError, float settleTime, float timeout);
  // Computes the PID output.
  float compute(float error);
  // Returns true if the PID has settled or timed out.
  bool isDone();
};