// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// A global instance of competition
competition Competition;


// ------------------------------------------------------------------------
//               Code below are not specific to any game
// ------------------------------------------------------------------------
 
int main() {
  // Register the autonomous and driver control functions.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Set up other button mapping for the controller
  setupButtonMapping();

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(200, msec);
  }
}
