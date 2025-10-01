using namespace vex;

void inTake();
void outTake();
void stopRollers();
void scoreLong();
void toggleHornPosition();;

extern motor rollerTop;
extern motor rollerBottom;
extern motor hornMotor;

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
extern int DRIVE_MODE;

void  setupButtonMapping();
void changeDriveMode();
void setChassisDefaults();
void usercontrol();
