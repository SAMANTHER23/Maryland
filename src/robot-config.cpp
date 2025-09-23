#include "vex.h"
using namespace vex;
// A global instance of brain used for printing to the V5 Brain screen
brain Brain;
controller controller1 = controller(primary);

// ------------------------------------------------------------------------
//              Drivetrain definition
// ------------------------------------------------------------------------
//If you only have 4  motors (or mecanum drive), assign leftMotor3, rightMotor3 to unused ports.
motor leftMotor1 = motor(PORT11, ratio18_1, true);//1 for one, 11 for two
motor leftMotor2 = motor(PORT12, ratio6_1, true);//2 for one, 12 for two
motor leftMotor3 = motor(PORT13, ratio6_1, true);//11 for one, 16 for two

motor rightMotor1 = motor(PORT1, ratio6_1, false);//4 for one, 1 for two
motor rightMotor2 = motor(PORT2, ratio6_1, false);//5 for one, 2 for two
motor rightMotor3 = motor(PORT3, ratio18_1, false);//13 for one, 3 for two

// inertial sensor for auton turning and heading
// If you do not have an inertial sensor, assign it to an unused port. Ignore the warning at the start of the program.
inertial inertial1 = inertial(PORT16);//13 for one, 19 for two

// 0: double arcade drive, 1: single aracde, 2: tank drive, 3: mecanum drive
int DRIVE_MODE = 0;


// ------------------------------------------------------------------------
//        Other subsystems: motors, sensors and helper functions definition
// ------------------------------------------------------------------------
motor rollerBottom = motor(PORT10, ratio6_1, false);//20 for one, 10 for two
motor rollerTop = motor(PORT14, ratio6_1, true);//19 for one, 14 for two


// total number of motors, including drivetrain
const int NUMBER_OF_MOTORS = 8;
void driveDistance() {
  // Placeholder function to avoid compilation errors.
}

void inTake() {
  rollerBottom.spin(forward, 12, volt);
  rollerTop.stop(hold);
}

void outTake() {
  rollerBottom.spin(forward, -12, volt);
  rollerTop.spin(forward, -12, volt);
}

void stopRollers() {
  // Stops the roller motors.
  rollerBottom.stop(brake);
  rollerTop.stop(brake);
}

void scoreLong() {
  rollerBottom.spin(forward, 11, volt);
  rollerTop.spin(forward, 11, volt);
}

// ------------------------------------------------------------------------
//               Code below are not specific to any game
// ------------------------------------------------------------------------


Drive chassis(
  //Left Motors:
  motor_group(leftMotor1, leftMotor2, leftMotor3),
  //Right Motors:
  motor_group(rightMotor1, rightMotor2, rightMotor3),
  //Inertial Sensor:
  inertial1,
  //wheel diameter:
  2.75,
  //Gear ratio of motor to wheel: if your motor has an 36-tooth gear and your wheel has a 48-tooth gear, this value will be 0.75.
  2.25 
);

// Resets the chassis constants.
void setChassisDefaults() {
  // Sets the heading of the chassis to the current heading of the inertial sensor.
  chassis.setHeading(chassis.gyro.heading());

  // Sets the drive PID constants for the chassis.
  // These constants are used to control the acceleration and deceleration of the chassis.
  chassis.setDrivePID(10, 1.5, 0, 10, 0);
  // Sets the turn PID constants for the chassis.
  // These constants are used to control the turning of the chassis.
  chassis.setTurnPID(10, 0.2, .015, 1.5, 7.5);
  // Sets the heading PID constants for the chassis.
  // These constants are used to control the heading adjustment of the chassis.
  chassis.setHeadingPID(6, .4, 1);
  // Sets the exit conditions for the drive functions.
  // These conditions are used to determine when the drive function should exit.
  chassis.setDriveExitConditions(1, 200, 2000);
  // Sets the exit conditions for the turn functions.
  // These conditions are used to determine when the turn function should exit.
  chassis.setTurnExitConditions(1.5, 200, 1500);

  // Sets the arcade drive constants for the chassis.
  // These constants are used to control the arcade drive of the chassis.
  chassis.setArcadeConstants(0.5, 0.5, 0.85);
}

void changeDriveMode(){
  controller1.rumble("-");
  DRIVE_MODE = (DRIVE_MODE +1)%3;
    switch (DRIVE_MODE) {
    case 0:
      printControllerScreen("Double Arcade");
      break;
    case 1:
      printControllerScreen("Single Arcade");
      break;
    case 2:
      printControllerScreen("Tank Drive");
      break;
    case 3:
      printControllerScreen("Mecanum Drive");
      break;
    }
}

// This is the user control function.
// It is called when the driver control period starts.
void usercontrol(void) {
  // Exits the autonomous menu.
  exitAuton();

  // This loop runs forever, controlling the robot during the driver control period.
  while (1) {
    if (!chassis.joystickTouched){
      if(controller1.Axis1.position() != 0 || controller1.Axis2.position() != 0 
        || controller1.Axis3.position() != 0 || controller1.Axis4.position() != 0) {
        chassis.joystickTouched = true;
      }
    }
    switch (DRIVE_MODE) {
    case 0: // double arcade
      chassis.controlArcade(controller1.Axis2.position(), controller1.Axis4.position());
      break;
    case 1: // single arcade
      chassis.controlArcade(controller1.Axis3.position(), controller1.Axis4.position());
      break;
    case 2: // tank drive
      chassis.controlTank(controller1.Axis3.position(), controller1.Axis2.position());      break;
    case 3: // mecanum drive
      chassis.controlMecanum(controller1.Axis4.position(), controller1.Axis3.position(), controller1.Axis2.position(), controller1.Axis1.position(), leftMotor1, leftMotor2, rightMotor1, rightMotor2);
      break;
    }

    // This wait prevents the loop from using too much CPU time.
    wait(20, msec);
   } 
}

