#include "vex.h"
PID::PID(float error, float kp, float kd) :
  error(error),
  kp(kp),
  kd(kd)
{};

PID::PID(float error, float kp, float ki, float kd, float starti, float settleError, float settleTime, float timeout) :
  error(error),
  kp(kp),
  ki(ki),
  kd(kd),
  starti(starti),
  settleError(settleError),
  settleTime(settleTime),
  timeout(timeout)
{};

float PID::compute(float error){
  if (fabs(error) < starti){ // StartI is used to prevent integral windup.
    accumulatedError+=error;
  }
  if ((error>0 && previousError<0)||(error<0 && previousError>0)){ 
    accumulatedError = 0; 
  } // This if statement checks if the error has crossed 0, and if it has, it eliminates the integral term.

  output = kp*error + ki*accumulatedError + kd*(error-previousError);

  previousError=error;

  if(fabs(error)<settleError){
    timeSpentSettled+=10;
  } else {
    timeSpentSettled = 0;
  }

  timeSpentRunning+=10;

  return output;
}

bool PID::isDone(){
  if (timeSpentRunning>timeout && timeout != 0){
    return(true);
  } 
  if (timeSpentSettled>settleTime){
    return(true);
  }
  return(false);
}