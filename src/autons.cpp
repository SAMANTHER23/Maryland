#include "vex.h"
int currentAutonSelection = 1;        // Current auton selection
int autonTestStep = 0;                // Current step in auton


// The first autonomous routine.
void auton1() {
  chassis.setHeading(180); // place the robot forwards
  chassis.driveDistance(-30, 6);
  rollerTop.spin(forward, 5, volt);
  wait(500, msec);
  stopRollers();
  chassis.driveDistance(9, 5);
  chassis.turnToHeading(270, 6);//was 
  inTake();
  chassis.driveDistance(13, 4);
  wait(1000, msec);
  stopRollers();
  chassis.turnToHeading(-135, 6);
  chassis.driveDistance(42, 6);
  chassis.turnToHeading(180, 6);
  chassis.driveDistance(-24, 6);
  scoreLong();
  wait(1050, msec);
  stopRollers();

}

// The second autonomous routine.
void auton2() {
  chassis.setHeading(0); // place the robot backwards
  chassis.driveDistance(-30, 6);
  rollerTop.spin(forward, 4, volt);
  wait(500, msec);
  stopRollers();
  chassis.driveDistance(8, 5);
  chassis.turnToHeading(-90, 6);//was 
  rollerBottom.spin(forward, 9, volt);
  chassis.driveDistance(13, 4);
  wait(1050, msec);
  stopRollers();
  chassis.turnToHeading(-45, 6);
  chassis.driveDistance(42, 6);
  chassis.turnToHeading(0, 6);
  chassis.driveDistance(-26, 6);
  rollerTop.spin(forward, 12, volt);
  wait(1500, msec);
  stopRollers();
}

// A long autonomous routine, e.g. skill.
// This routine is broken into steps to allow for testing of individual steps.
// This allows for easier debugging of individual parts of the long autonomous routine.
void sampleSkill() {
  if (autonTestStep == 0) 
  {
    chassis.turnToHeading(180);
    if (!continueAutonStep()) return; // If in test mode, stop here for testing.
  }
  if (autonTestStep == 1)  
  {
    chassis.driveDistance(5);
    chassis.turnToHeading(chassis.getHeading() + 90); // Turn right
    if (!continueAutonStep()) return; // If in test mode, stop here for testing.
  } 
  if(autonTestStep == 2) 
  {
    chassis.turnToHeading(chassis.getHeading() - 90); // Turn left
    chassis.driveDistance(-5);
  }
}

// Runs the selected autonomous routine.
void runAutonItem() {
  switch (currentAutonSelection) {
  case 0:
    auton1();
    break;
  case 1:
    auton2();
    break;
  case 2:
    sampleSkill();
    break;
    }
}

// The names of the autonomous routines to be displayed in the menu.
char const * autonMenuText[] = {
  "auton1",
  "auton2",
  "auton_skill"
};



// ----------------------------------------------------------------------------
//               Code below are not specific to any game
// ----------------------------------------------------------------------------

int autonNum;                         // Total number of autons, automatically calculated based on the size of the autonMenuText array
bool autonTestMode = true;           // Indicates if in test mode
bool exitAutonMenu = true;           // Flag to exit the autonomous menu
bool enableEndGameTimer = false;      // Flag to indicate if endgame timer is enabled 
const int END_GAME_SECONDS = 85;      // Endgame reminder starts at 85 seconds

// The function is called when the autonomous period starts.
void autonomous(void) {
  // Exits the autonomous menu.
  exitAutonMenu = true;
  enableEndGameTimer = true;
  // Runs the selected autonomous routine.
  runAutonItem();
}

// This function prints the selected autonomous routine to the brain and controller screens.
void printMenuItem() {
  if (currentAutonSelection < 0) {
    printControllerScreen("quick test");
    return;
  }    
  // Clears the brain screen.
  Brain.Screen.clearScreen();
  // Sets the cursor to the third row, first column.
  Brain.Screen.setCursor(3, 1);
  // Prints the selected autonomous routine name.
  Brain.Screen.print("%s", autonMenuText[currentAutonSelection]);
  printControllerScreen(autonMenuText[currentAutonSelection]);
}

// This function displays the autonomous menu on the brain screen.
void showAutonMenu() {
  autonNum = sizeof(autonMenuText) / sizeof(autonMenuText[0]);
  autonTestStep = 0;

  Brain.Screen.setFont(mono30);
  printMenuItem();

  // This loop runs until the autonomous menu is exited.
  while (!exitAutonMenu) {
    // If the brain screen is pressed, cycle through the autonomous routines.
    if (Brain.Screen.pressing()) {
      // Waits until the finger is lifted up from the screen.
      while (Brain.Screen.pressing()) {
        wait(20, msec);
      }
      // Cycles through the autonomous routines.
      currentAutonSelection = (currentAutonSelection + 1) % autonNum;
      printMenuItem();
      controller1.rumble(".");
    }
    // This wait prevents the loop from using too much CPU time.
    wait(50, msec);
  }
  Brain.Screen.setFont(mono20);
}

// This function is a thread that runs in the background to remind the driver of the end game.
void endgameTimer() {
  // Waits until the end game starts.
  while (Brain.Timer.time(sec) < END_GAME_SECONDS) {
    wait(500, msec);
  }
  if (enableEndGameTimer)
  {
    printControllerScreen("end game");
    controller1.rumble("-");
  }

  // Checks the motors health every 60 seconds in drive practice
  while(true)
  {
    wait(60, seconds);
    if (!autonTestMode) checkMotors(NUMBER_OF_MOTORS);
  }
}

void exitAuton()
{
  exitAutonMenu = true;
  chassis.joystickTouched = false;
    // Clears the brain timer.
  Brain.Timer.clear();
    // Starts the end game timer thread.
  thread endgameTimer_thread = thread(endgameTimer);
  if (!chassis.joystickTouched) {
    //TODO: some macto actions
  }
  chassis.stop(coast);
}

bool setupGyro() {
  wait(100, msec);
  if (!chassis.gyro.installed()) {
    printControllerScreen("inertial sensor failure");
    controller1.rumble("---");
    wait(2, seconds);
    return false;  
  }

  chassis.gyro.calibrate(3);
  // Waits until the inertial sensor is calibrated.
  while (chassis.gyro.isCalibrating()) {
    wait(100, msec);
  }
  // Rumbles the controller to indicate that the gyro is calibrated.
  controller1.rumble(".");
  return true;
}

// This function is called before the autonomous period starts.
void pre_auton() {
  // Sets up the gyro.
  bool gyroSetupSuccess = setupGyro();

  bool motorsSetupSuccess = true;

  // Checks the motors.
  motorsSetupSuccess = checkMotors(NUMBER_OF_MOTORS);
  //set the parameters for the chassis
  setChassisDefaults();
  // Shows the autonomous menu.
  if(gyroSetupSuccess && motorsSetupSuccess) showAutonMenu();
}



// ----------------------------------------------------------------------------
//                 For testing autonomous steps
// ----------------------------------------------------------------------------

bool continueAutonStep()
{
  autonTestStep++;
  if (autonTestMode) return false; // If in test mode, stop here for testing.
  return true; 
}

// This function is called when the Right button is pressed.
void buttonRightAction()
{
  if ((Brain.Timer.time(sec) < 5) && !autonTestMode) {  
    // If the button is pressed within 5 seconds of starting the program, enter test mode.
    controller1.rumble("-");
    printControllerScreen("Test Mode: ON");
    wait(1, sec);
    showAutonMenu();
    autonTestMode = true;
    return;
  } 
  if (autonTestMode)
  {
    controller1.rumble(".");
    // if in test mode, scroll through the auton menu
    currentAutonSelection = (currentAutonSelection + 1) % autonNum;
    showAutonMenu();
  }
}

void buttonLeftAction()
{
  if ((Brain.Timer.time(sec) < 5)) {
    // If the button is pressed within 5 seconds of starting the program, change the drive mode.
    changeDriveMode();
    return;
  }
  if (autonTestMode) 
  {
    // if in test mode, scroll through the auton menu
    controller1.rumble(".");
    currentAutonSelection = (currentAutonSelection - 1 + autonNum) % autonNum;
    showAutonMenu();
    return;
  }
}


void buttonDownAction()
{
  if (autonTestMode) 
  {
    // If in test mode, go to the next step.
    controller1.rumble(".");
    autonTestStep++;
    char msg[30];
    sprintf(msg, "Step: %d", autonTestStep);
    printControllerScreen(msg);
  }
}

void buttonUpAction()
{
  if (autonTestMode) 
  {
    // If in test mode, go to the previous step.
    controller1.rumble(".");
    if (autonTestStep > 0) autonTestStep--;
    char msg[30];
    sprintf(msg, "Step: %d", autonTestStep);
    printControllerScreen(msg);
  }
}

void buttonAAction()
{
  if (autonTestMode) 
  {
    // If in test mode, run the selected autonomous routine for testing and displays the run time.
    controller(primary).rumble(".");
    double t1 = Brain.Timer.time(sec);
    runAutonItem(); 
    double t2 = Brain.Timer.time(sec);
    char timeMsg[30];
    sprintf(timeMsg, "run time: %.1f", t2-t1);
    printControllerScreen(timeMsg);
    chassis.stop(coast);
  }
}

// Register the controller button callbacks for autonomous testing.
void registerAutonTestButtons()
{
  controller1.ButtonRight.pressed(buttonRightAction);
  controller1.ButtonLeft.pressed(buttonLeftAction);
  controller1.ButtonDown.pressed(buttonDownAction);
  controller1.ButtonUp.pressed(buttonUpAction);
  controller1.ButtonA.pressed(buttonAAction);
}