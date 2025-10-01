#include "vex.h"
PID::PID(float kp, float kd) :
  kp(kp),
  kd(kd)
{};

PID::PID(float kp, float ki, float kd, float starti, float settleError, float settleTime, float timeout) :
  kp(kp),
  ki(ki),
  kd(kd),
  starti(starti),
  settleError(settleError),
  settleTime(settleTime),
  timeout(timeout)
{};

float PID::update(float error){
  if (fabs(error) < starti){ // starti is used to prevent integral windup.
    sumError+=error;
  }
  if ((error>0 && previousError<0)||(error<0 && previousError>0)){ 
    sumError = 0; 
  } // Eliminates the integral term if the error crosses zero.

  float output = kp*error + ki*sumError + kd*(error-previousError);
  previousError=error;

  if(fabs(error)<settleError){
    timeSettleTime+=10;
  } else {
    timeSettleTime = 0;
  }
  timeTimout+=10;

  return output;
}

bool PID::isDone(){
  if (timeTimout > timeout && timeout != 0){
    return true;
  } 
  if (timeSettleTime > settleTime){
    return true;
  }
  return false;
}