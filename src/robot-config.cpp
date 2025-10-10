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
motor hornMotor = motor(PORT8, ratio18_1, true);//19 for one, 14 for two


// total number of motors, including drivetrain
const int NUMBER_OF_MOTORS = 8;

bool hornUp = false;
void toggleHornPosition() {
  hornUp = !hornUp;
  if (hornUp) {
    hornMotor.spin(forward, 8, volt);
    wait(100, msec);
    waitUntil(hornMotor.torque()>0.4);
    hornMotor.stop(brake);
    chassis.stop(coast);
  } else {
    hornMotor.setVelocity(75, percent);
    hornMotor.setTimeout(1000, msec);
    hornMotor.spinFor(reverse, 300, degrees, false);
    chassis.stop(hold);
  }
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
  chassis.stop(coast);
}

void scoreLong() {
  rollerBottom.spin(forward, 11, volt);
  rollerTop.spin(forward, 11, volt);
  chassis.stop(hold);
}



// ------------------------------------------------------------------------
//              Button controls
// ------------------------------------------------------------------------

// This function is called when the L1 button is pressed.
void buttonL1Action() {
  inTake();
  
  // Wait until the button is released to stop the rollers.
  while(controller1.ButtonL1.pressing()) {
    if (controller1.ButtonR2.pressing()) outTake();
    wait (20, msec);
  }
  stopRollers();
}

void buttonL2Action() {
  scoreLong();
  // Wait until the button is released to stop the rollers.
  while(controller1.ButtonL2.pressing()) {
    wait (20, msec);
    if(controller1.ButtonR2.pressed){
      rollerTop.spin(forward, 6, volt);
      rollerBottom.spin(forward, 6, volt);
    }
    elif(controller1.ButtonR1.pressed){
      rollerBottom.spin(backwards, 6, volt);
    }
  }
  stopRollers();
}

void buttonR1Action() {
  toggleHornPosition();
}

void buttonR2Action()
{
  // brake the drivetrain until the button is released.
  chassis.stop(hold);
  controller1.rumble(".");
  waitUntil(!controller1.ButtonR2.pressing());
  chassis.checkStatus();
  chassis.stop(coast);
}

bool macroMode = false;
void buttonAAction() {
  if(macroMode) return;
  macroMode = true;
  float currentHeading = chassis.inertialSensor.heading();

  if(hornUp) {
    chassis.driveDistance(10, 10, currentHeading, 6);
    chassis.turnToHeading(currentHeading-90);
    chassis.driveDistance(13);
    toggleHornPosition();
    chassis.turnToHeading(currentHeading);
    chassis.driveDistance(-24);
  }
  else {
    chassis.driveDistance(-24, 10, currentHeading, 6);
  }
  macroMode = false;
 
}

void setupButtonMapping() {
  controller1.ButtonL1.pressed(buttonL1Action);
  controller1.ButtonR1.pressed(buttonR1Action);
  controller1.ButtonL2.pressed(buttonL2Action);
  controller1.ButtonR2.pressed(buttonR2Action);
  controller1.ButtonA.pressed(buttonAAction);
}




// ------------------------------------------------------------------------
//               chassis parameters and PID constants
// ------------------------------------------------------------------------

Drive chassis(
  //Left Motors:
  motor_group(leftMotor2, leftMotor1, leftMotor3),
  //Right Motors:
  motor_group(rightMotor2, rightMotor1, rightMotor3),
  //Inertial Sensor:
  inertial1,
  //wheel diameter:
  2.75,
  //Gear ratio of motor to wheel: if your motor has an 36-tooth gear and your wheel has a 48-tooth gear, this value will be 0.75.
  0.75
);

// Resets the chassis constants.
void setChassisDefaults() {
  // Sets the heading of the chassis to the current heading of the inertial sensor.
  chassis.setHeading(chassis.inertialSensor.heading());

  chassis.setMaxVoltage(10, 10, 6);
  // Sets the drive PID constants for the chassis.
  // These constants are used to control the acceleration and deceleration of the chassis.
  chassis.setDrivePID(1.5, 0, 10, 0);
  // Sets the turn PID constants for the chassis.
  // These constants are used to control the turning of the chassis.
  chassis.setTurnPID(0.2, .015, 1.5, 7.5);
  // Sets the heading PID constants for the chassis.
  // These constants are used to control the heading adjustment of the chassis.
  chassis.setHeadingPID(0.4, 1);
  // Sets the exit conditions for the drive functions.
  // These conditions are used to determine when the drive function should exit.
  chassis.setDriveExitConditions(1, 300, 3000);
  // Sets the exit conditions for the turn functions.
  // These conditions are used to determine when the turn function should exit.
  chassis.setTurnExitConditions(1.5, 300, 3000);

  // Sets the arcade drive constants for the chassis.
  // These constants are used to control the arcade drive of the chassis.
  chassis.setArcadeConstants(0.5, 0.5, 0.85);
}


// ------------------------------------------------------------------------
//              Drive modes and user control
// ------------------------------------------------------------------------

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

