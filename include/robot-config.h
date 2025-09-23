using namespace vex;

extern motor roller;

void inTake();
void outTake();
void stopRollers();
void scoreLong();
void driveDistance();;

extern motor rollerTop;
extern motor rollerBottom;


// ------------------------------------------------------------------------
//               Only change code below this line when necessary
// ------------------------------------------------------------------------

// A global instance of the brain used for printing to the V5 Brain screen.
extern brain Brain;
extern controller controller1;
// Forward declaration of the Drive class.
class Drive;
// A global instance of the Drive class.
extern Drive chassis;

extern const int NUMBER_OF_MOTORS;

void changeDriveMode();
void setChassisDefaults();
void usercontrol();
