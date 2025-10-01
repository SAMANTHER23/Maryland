#include "vex.h"
int currentAutonSelection = 0;        // Current auton selection
int autonTestStep = 0;                // Current step in auton test mode


void autonLeft() {
  //Place robot facing backwards on left side of field
  chassis.setHeading(180);

  //score preloaded ball
  chassis.driveDistance(-30);
  wait(50, msec);
  scoreLong();
  wait(50, msec);
  stopRollers();

  //go get 3 balls
  chassis.driveDistance(9);
  chassis.turnToHeading(270);
  inTake();
  chassis.driveDistance(20, 4);
  chassis.turnToHeading(-130);
  stopRollers();

//go score long goal
  chassis.driveDistance(29);
  chassis.turnToHeading(180);
  chassis.driveDistance(-12);
  scoreLong();
  wait(1000, msec);
  stopRollers();

  //horn push
  toggleHornPosition();
  chassis.driveDistance(10);
  chassis.turnToHeading(90);
  chassis.driveDistance(12);
  toggleHornPosition();
  chassis.turnToHeading(180);
  chassis.driveDistance(-24);

}

// The second autonomous routine.
void autonRight() {
  //Score preloaded ball
 chassis.setHeading(0); 
  chassis.driveDistance(30);
  wait(50, msec);

  //grab three balls
  chassis.driveDistance(-7, 5);
  chassis.turnToHeading(90, 6);//was 
  inTake();
  chassis.driveDistance(14, 4);
  wait(1000, msec);
  stopRollers();

  //Score long goal
  chassis.turnToHeading(135, 6);
  chassis.driveDistance(40, 6);
  chassis.turnToHeading(180, 6);
  chassis.driveDistance(-26, 6);
  scoreLong();
  wait(1500, msec);
  stopRollers();

  //horn push
  chassis.setHeading(180);
  toggleHornPosition();
  chassis.driveDistance(10);
  chassis.turnToHeading(90);
  chassis.driveDistance(12);
  toggleHornPosition();
  chassis.turnToHeading(180);
  chassis.driveDistance(-24);
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
   autonLeft();
    break;
  case 1:
    autonRight();
    break;
  }
}

// The names of the autonomous routines to be displayed in the menu.
char const * autonMenuText[] = {
  "left",
  "right"
};



// ----------------------------------------------------------------------------
//               Code below are not specific to any game
// ----------------------------------------------------------------------------

int autonNum;                         // Total number of autons, automatically calculated based on the size of the autonMenuText array
bool autonTestMode = false;           // Indicates if in test mode
bool configMode = false;              // Indicates if in configuration mode
bool exitAutonMenu = false;           // Flag to exit the autonomous menu
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
    // Clears the brain timer.
  Brain.Timer.clear();
    // Starts the end game timer thread.
  thread endgameTimer_thread = thread(endgameTimer);
  chassis.stop(coast);
}

bool setupinertialSensor() {
  wait(100, msec);
  if (!chassis.inertialSensor.installed()) {
    printControllerScreen("inertial sensor failure");
    controller1.rumble("---");
    wait(2, seconds);
    return false;  
  }

  chassis.inertialSensor.calibrate(3);
  // Waits until the inertial sensor is calibrated.
  while (chassis.inertialSensor.isCalibrating()) {
    wait(100, msec);
  }
  // Rumbles the controller to indicate that the inertialSensor is calibrated.
  controller1.rumble(".");
  return true;
}

/**
 * @brief Trim leading and trailing whitespace from a string.
 * @param str The string to trim.
 * @return A pointer to the trimmed string.
 */
char* trim_whitespace(char* str) {
    char* end;
    // Trim leading whitespace
    while (isspace((unsigned char)*str)) {
        str++;
    }
    if (*str == 0) {
        return str; // All whitespace
    }
    // Trim trailing whitespace
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = 0;
    return str;
}

// load program parameters from the SD card
void loadConfigParameters()
{
  if (Brain.SDcard.isInserted()) {
    if (Brain.SDcard.exists("parameters.txt")) {  
      // open the file for reading
      uint8_t  myReadBuffer[1000];  
      Brain.SDcard.loadfile("parameters.txt", myReadBuffer, sizeof(myReadBuffer));
      wait(0.5, seconds);

      char line_buffer[256];
      char* buffer_ptr = (char*)myReadBuffer;
      char* line_end;

      // Process the buffer line by line
      while ((line_end = strchr(buffer_ptr, '\n')) != NULL) {
          // Copy the current line into the line_buffer
          int line_len = line_end - buffer_ptr;
          strncpy(line_buffer, buffer_ptr, line_len);
          line_buffer[line_len] = '\0';

          // Find the position of the '=' character
          char* equals_sign = strchr(line_buffer, '=');
          if (equals_sign != NULL) {
              // Null-terminate the key part of the string
              *equals_sign = '\0';
              
              // Extract the key and value strings
              char* key = trim_whitespace(line_buffer);
              char* value_str = trim_whitespace(equals_sign + 1);

              // Check if the line is for 'auton' and 'drive_mode'
              if (strcmp(key, "auton") == 0) {
                  currentAutonSelection = atoi(value_str);
              } else if (strcmp(key, "drive_mode") == 0) {
                  DRIVE_MODE = atoi(value_str);
              }
          }
          
          // Move the pointer to the start of the next line
          buffer_ptr = line_end + 1;
      }
    }
  }
}

// save program parameters to the SD card
void saveConfigParameters()
{
  if (Brain.SDcard.isInserted()) {    
    // Create the parameter string with current values
    char parameter_buffer[256];
    sprintf(parameter_buffer, "auton = %d\ndrive_mode = %d\n", currentAutonSelection, DRIVE_MODE);
    int32_t result = Brain.SDcard.savefile("parameters.txt", (uint8_t*)parameter_buffer, strlen(parameter_buffer));
    
    if (result < 0) {
      printControllerScreen("save failed");
    }
    wait(0.5, seconds);
  } 
}

// This function is called before the autonomous period starts.
void pre_auton() {
  // Sets up the inertialSensor.
  bool inertialSensorSetupSuccess = setupinertialSensor();

  bool motorsSetupSuccess = true;

  // Checks the motors.
  motorsSetupSuccess = checkMotors(NUMBER_OF_MOTORS);
  //set the parameters for the chassis
  setChassisDefaults();
  // load parameters from the SD card
  loadConfigParameters();
  // Shows the autonomous menu and register the buttons for autonomous testing.
  if(inertialSensorSetupSuccess && motorsSetupSuccess) {
    showAutonMenu();
    registerAutonTestButtons();
  }
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

void autonTestButtonCheck()
{
    // within 5 seconds of driver control.
  while (Brain.Timer.time(sec) < 5) {
    if (controller1.ButtonRight.pressing())
    {
      waitUntil(!controller1.ButtonRight.pressing());
      //activate the auton test mode
      controller1.rumble("-");
      printControllerScreen("Test Mode: ON");
      wait(1, sec);
      showAutonMenu();
      autonTestMode = true;
    }
    if (controller1.ButtonLeft.pressing())
    {
      //active the configuration mode to change drive mode
      waitUntil(!controller1.ButtonLeft.pressing());
      controller1.rumble("-");
      printControllerScreen("Config Mode: ON");
      configMode = true;
    }
    wait(100, msec);
  }
  while(true)
  {
    if (configMode) {
      if(controller1.ButtonLeft.pressing())
      {
        waitUntil(!controller1.ButtonLeft.pressing());
        // Change the drive mode
        controller1.rumble("-");
        changeDriveMode();
        wait(0.5, sec);
        saveConfigParameters();
      }
    }

    if (autonTestMode) 
    {
      if(controller1.ButtonRight.pressing())
      {
        waitUntil(!controller1.ButtonRight.pressing());
        controller1.rumble(".");
        // Scroll through the auton menu
        currentAutonSelection = (currentAutonSelection + 1) % autonNum;
        wait(0.5, sec);
        saveConfigParameters();
        showAutonMenu();
      }
      if(controller1.ButtonLeft.pressing())
      {
        waitUntil(!controller1.ButtonLeft.pressing());
        controller1.rumble(".");
        // Scroll through the auton menu
        currentAutonSelection = (currentAutonSelection - 1 + autonNum) % autonNum;
        wait(0.5, sec);
        saveConfigParameters();
        showAutonMenu();
      }
      if(controller1.ButtonDown.pressing())
      {
        waitUntil(!controller1.ButtonDown.pressing());
        controller1.rumble(".");
        // Go to the next step.
        autonTestStep++;
        char msg[30];
        sprintf(msg, "Step: %d", autonTestStep);
        printControllerScreen(msg);
      }
      if(controller1.ButtonUp.pressing())
      {
        waitUntil(!controller1.ButtonUp.pressing());
        controller1.rumble(".");
        // Go to the previous step.
        if (autonTestStep > 0) autonTestStep--;
        char msg[30];
        sprintf(msg, "Step: %d", autonTestStep);
        printControllerScreen(msg);
      }
      if(controller1.ButtonA.pressing())
      {
        waitUntil(!controller1.ButtonA.pressing());
        //run the selected autonomous routine for testing and displays the run time.
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
    wait(100, msec);
  }
}

// in background, check the controller button press for autonomous testing.
void registerAutonTestButtons()
{
  thread autonTestButtonThread = thread(autonTestButtonCheck);
}