#pragma once
#include "vex.h"
#include <string>

// A class to control the robot's drivetrain.
class Drive
{
private:
  // The diameter of the wheels.
  float wheelDiameter;
  // The gear ratio of the drivetrain.
  float gearRatio;
  // The ratio to convert from inches to degrees of wheel rotation.
  float driveInToDegRatio;

  // PID constants for turning.
  float turnMaxVoltage;
  float turnKp;
  float turnKi;
  float turnKd;
  float turnStarti;

  // turn exit conditions.
  float turnSettleError = 1.5;
  float turnSettleTime = 200;
  float turnTimeout = 1500;

  // PID constants for driving.
  float driveMaxVoltage;
  float driveKp;
  float driveKi;
  float driveKd;
  float driveStarti;

  // drive exit conditions.
  float driveSettleError = 1;
  float driveSettleTime = 200;
  float driveTimeout = 2000;

  // PID constants for maintaining heading while driving.
  float headingMaxVoltage;
  float headingKp;
  float headingKd;

  // Constants for arcade drive.
  float kBrake = 0.5;
  float kTurnBias = 0.5; 
  float kTurnDampingFactor = 0.85;

  // allows for a non-proportional steering response
  float kThrottle = 5;
  float kTurn = 10;

  // The default brake type for the drivetrain.
  vex::brakeType stopMode = coast;

  // The motor group for the left side of the drivetrain.
  motor_group leftDrive;
  // The motor group for the right side of the drivetrain.
  motor_group rightDrive;

    // Gets the position of the left side of the drivetrain in inches.
  float getLeftPositionIn();
  // Gets the position of the right side of the drivetrain in inches.
  float getRightPositionIn();


public: 
  // The inertial sensor.
  inertial gyro;

// The desired heading of the robot.
  float desiredHeading;

  // The constructor for the Drive class.
  Drive(motor_group leftDrive, motor_group rightDrive, inertial gyro, float wheelDiameter, float gearRatio);

  // Gets the current heading of the robot.
  float getHeading();
  // Sets the current heading of the robot.
  void setHeading(float orientationDeg);

  // Drives the robot with a specific voltage for each side of the drivetrain.
  void driveWithVoltage(float leftVoltage, float rightVoltage);

  // Turns the robot to a specific heading.
  void turnToHeading(float heading);
  // Turns the robot to a specific heading with a maximum voltage.
  void turnToHeading(float heading, float turnMaxVoltage);

  // Drives the robot a specific distance.
  void driveDistance(float distance);
  // Drives the robot a specific distance with a maximum voltage.
  void driveDistance(float distance, float driveMaxVoltage);
  // Drives the robot a specific distance while turning to a heading.
  void driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage);

  // A flag to indicate if the drivetrain needs to be stopped.
  bool drivetrainNeedsStopped = false;
  bool joystickTouched = false;

  // Controls the robot in arcade mode.
  void controlArcade(int throttle, int turn);
  // Controls the robot in tank mode.
  void controlTank(int left, int right);
  void controlMecanum(int x, int y, int acc, int steer, motor DriveLF, motor DriveLR, motor DriveRF, motor DriveRB);

  // Sets the PID constants for driving.
  void setDrivePID(float driveMaxVoltage, float driveKp, float driveKi, float driveKd, float driveStarti);
  // Sets the exit conditions for driving.
  void setDriveExitConditions(float driveSettleError, float driveSettleTime, float driveTimeout);
  // Sets the PID constants for maintaining heading.
  void setHeadingPID(float headingMaxVoltage, float headingKp, float headingKd);
  // Sets the exit conditions for turning.
  void setTurnExitConditions(float turnSettleError, float turnSettleTime, float turnTimeout);
  // Sets the PID constants for turning.
  void setTurnPID(float turnMaxVoltage, float turnKp, float turnKi, float turnKd, float turnStarti); 
  // Sets the constants for arcade drive.
  void setArcadeConstants(float kBrake, float kTurnBias, float kTurnDampingFactor);

  // Stops the drivetrain.
  void stop(vex::brakeType mode);

  void checkStatus();
};