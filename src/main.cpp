// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// A global instance of competition
// This object is used to register callbacks for the autonomous and driver control periods.
competition Competition;

// This function is called when the L1 button is pressed.
void buttonL1Action() {
  inTake();
  
  // Wait until the button is released to stop the rollers.
  while(controller1.ButtonL1.pressing()) {
    wait (20, msec);
  }
  stopRollers();
}

void buttonL2Action() {
  scoreLong();
  // Wait until the button is released to stop the rollers.
  while(controller1.ButtonL2.pressing()) {
    wait (20, msec);
  }
  stopRollers();
}

void buttonR1Action() {
  outTake();
  
  // Wait until the button is released to stop the rollers.
  while(controller1.ButtonR1.pressing()) {
    wait (20, msec);
  }
  stopRollers();
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

void setupButtonMapping() {
  controller1.ButtonL1.pressed(buttonL1Action);
  controller1.ButtonR1.pressed(buttonR1Action);
  controller1.ButtonL2.pressed(buttonL2Action);
  controller1.ButtonR2.pressed(buttonR2Action);
}



// ------------------------------------------------------------------------
//               Code below are not specific to any game
// ------------------------------------------------------------------------
  
void pollCommandMessages()
{
  static FILE* serialPort = nullptr;
  
  // Open port once
  if (serialPort == nullptr) {
    serialPort = fopen("/dev/serial1", "r");
    if (serialPort == nullptr) return;
  }

  char buffer[256];
  if (fgets(buffer, sizeof(buffer), serialPort) == nullptr) return;

  // Simple trim and check
  std::string command(buffer);
  command.erase(0, command.find_first_not_of(" \t\r\n"));
  command.erase(command.find_last_not_of(" \t\r\n") + 1);
  
  if (command.empty()) return;

  controller(primary).rumble(".");
  printControllerScreen(command.c_str());

  // Parse command
  size_t spacePos = command.find(' ');
  std::string cmd = command.substr(0, spacePos);
  std::string params = spacePos != std::string::npos ? command.substr(spacePos + 1) : "";

  // Execute command
  if (cmd == "drive") {
    chassis.driveDistance(atof(params.c_str()), 6);
  } else if (cmd == "turn") {
    chassis.turnToHeading(atof(params.c_str()), 6);
  } else if (cmd == "set_heading") {
    chassis.setHeading(atof(params.c_str()));
  }
  
  chassis.stop(coast);
}

int main() {
  // Register the autonomous and driver control functions.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  //comment out the following line to disable auton testing
  registerAutonTestButtons();

  // Set up other button mapping for the controller
  setupButtonMapping();

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    // comment out the following line to disable remote command processing
    pollCommandMessages();
    wait(200, msec);
  }
}
