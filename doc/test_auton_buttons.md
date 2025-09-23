# Test Auton Button System

## Overview

The test auton system allows you to test autonomous routines during driver control with step-by-step execution capability. It uses multiple buttons for navigation and execution control.

## Complete Flow of Actions

### 1. Entering Test Mode
**Button: Right** (within first 5 seconds of program startup)

```cpp
if ((Brain.Timer.time(sec) < 5) && !autonTestMode) {  
  // If the button is pressed within 5 seconds of starting the program, enter test mode.
  controller1.rumble("-");
  printControllerScreen("Test Mode: ON");
  wait(1, sec);
  showAutonMenu();
  autonTestMode = true;
  return;
}
```

**What happens:**
- ✅ Checks if program has been running for less than 5 seconds
- ✅ Checks if not already in test mode
- ✅ Controller rumbles with "-" pattern
- ✅ Displays "Test Mode: ON" on controller screen
- ✅ Shows auton menu
- ✅ Sets `autonTestMode = true`

---

### 2. Navigating the Auton Menu
**Button: Right** (when already in test mode)

```cpp
if (autonTestMode) {
  controller1.rumble(".");
  // if in test mode, scroll through the auton menu
  currentAutonSelection = (currentAutonSelection + 1) % autonNum;
  showAutonMenu();
}
```

**What happens:**
- ✅ Increments `currentAutonSelection` (cycles through available autons)
- ✅ Calls `showAutonMenu()` which displays menu on brain and controller screens
- ✅ Shows current selection (e.g., "auton1", "auton2", "auton_skill")

**Navigation Options:**
- **Right Button**: Move forward in menu (auton1 → auton2 → auton_skill → auton1...)
- **Left Button**: Move backward in menu (auton_skill → auton2 → auton1 → auton_skill...)

---

### 3. Step-by-Step Navigation
**Button: Up/Down** (when in test mode)

```cpp
// Up Button - Previous Step
if (autonTestMode) {
  controller1.rumble(".");
  if (autonTestStep > 0) autonTestStep--;
  controller1.Screen.print("Step: %d         ", autonTestStep);
}

// Down Button - Next Step  
if (autonTestMode) {
  controller1.rumble(".");
  autonTestStep++;
  controller1.Screen.print("Step: %d           ", autonTestStep);
}
```

**What happens:**
- ✅ **Up Button**: Decreases step number (if > 0)
- ✅ **Down Button**: Increases step number
- ✅ Controller rumbles to confirm action
- ✅ Displays current step on controller screen

---

### 4. Running the Selected Auton/Step
**Button: A** (when in test mode)

```cpp
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
    sprintf(timeMsg, "run time: %.0f", t2-t1);
    printControllerScreen(timeMsg);
    chassis.stop(coast);
  }
}
```

**What happens:**
- ✅ Checks if in test mode
- ✅ Records start time
- ✅ Calls `runAutonItem()` which executes the selected auton
- ✅ Calculates and displays run time
- ✅ Stops chassis with coast
- ✅ Controller rumbles to confirm execution

---

### 5. Step-by-Step Execution System

**Auton Functions with Step Control:**
```cpp
void sampleSkill() {
  if (autonTestStep == 1) {
    chassis.turnToHeading(180);
    if (!continueAutonStep()) return; // Stop here if in test mode
  }
  if (autonTestStep == 2) {
    chassis.driveDistance(5);
    chassis.turnToHeading(chassis.getHeading() + 90);
    if (!continueAutonStep()) return; // Stop here if in test mode
  } 
  if(autonTestStep == 3) {
    chassis.turnToHeading(chassis.getHeading() - 90);
    chassis.driveDistance(-5);
  }
}
```

**Step Control Function:**
```cpp
bool continueAutonStep() {
  autonTestStep++;
  if (autonTestMode) {
    return false; // Stop execution in test mode
  }
  return true; // Continue execution in normal mode
}
```

**What happens:**
- ✅ Each step checks `autonTestStep` to determine what to execute
- ✅ `continueAutonStep()` increments step and returns false in test mode
- ✅ This causes the function to return early, stopping execution
- ✅ In normal mode, execution continues through all steps


---

### 6. Abort auto driving
- ✅ At any time, move joystick to abort the auto driving.

---

### 7. Show status
**Button: R2**
- ✅ If hold this button while driving, upon releasing it, the controller displays current heading and distance driven.

## Button Summary

| Button | Action | When |
|--------|--------|------|
| **Right** | Enter test mode | Within 5 seconds of startup |
| **Right** | Next auton in menu | When in test mode |
| **Left** | Previous auton in menu | When in test mode |
| **Up** | Previous step | When in test mode |
| **Down** | Next step | When in test mode |
| **A** | Run selected auton/step | When in test mode |
| **Movement of Joystick** | Abort auto driving | Always |
| **R2** | Show status | Always |

---

## Example Flow

1. **Start program** → Robot boots up
2. **Press Right** (within 5 seconds) → Enter test mode, see "auton1" on screen
3. **Press Right** → See "auton2" on screen  
4. **Press Down** → Step increases to 1
5. **Press A** → Run step 1 of auton2, see "run time: X" on screen
6. **Press Up** → Step decreases to 0
7. **Press A** → Run step 0 of auton2
8. **Press Left** → Back to "auton1" in auton menu

---

## 🔧 Technical Details

### Global Variables
```cpp
bool autonTestMode = false;         // Indicates if in test mode
int currentAutonSelection = 0;      // Current auton selection
int autonTestStep = 0;              // Current step in auton
int autonNum;                       // Total number of autons (automatically calculated)
```

### Key Functions
- `registerAutonTestButtons()`: Registers all button callbacks for auton testing
- `buttonRightAction()`: Handles Right button (enter test mode, next auton)
- `buttonLeftAction()`: Handles Left button (change drive mode, previous auton)
- `buttonUpAction()`: Handles Up button (previous step)
- `buttonDownAction()`: Handles Down button (next step)
- `buttonAAction()`: Handles A button (run auton test)
- `continueAutonStep()`: Controls step progression

### Button Registration
The auton testing buttons are automatically registered in the main function:

```cpp
int main() {
  // Register the autonomous and driver control functions.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Register auton testing button callbacks
  registerAutonTestButtons();


```

### Menu System
The menu system automatically calculates the number of available autons based on the `autonMenuText` array in `autons.cpp`:

```cpp
char const * autonMenuText[] = {
    "auton1",
    "auton2", 
    "auton_skill"
};
```

### Available Autons
- **auton1**: Simple forward movement
- **auton2**: Basic movement with heading
- **auton_skill**: Complex multi-step routine with step-by-step testing

---

## Tips for Using

1. **Quick Testing**: Use this system to rapidly test auton changes
2. **Step-by-Step**: Use Up/Down buttons to navigate through steps
3. **Menu Navigation**: Use Right/Left to quickly switch between different autons
4. **Auton Execution**: Use A button to run the selected auton and see execution time
5. **Timing**: Remember to press Right within 5 seconds of program startup
6. **Safety**: Driver control can abort the auton execution at any time
7. **Step Control**: Each auton can have different numbers of steps

---

## Benefits

- **Rapid Iteration**: Quickly test different auton routines
- **Step Debugging**: Navigate through individual steps for precise testing
- **Menu System**: Easy navigation between multiple autons
- **Flexible Testing**: Test individual steps or full autons
- **Visual Feedback**: Clear indication of current step and auton
- **Timing Feedback**: See execution time for performance optimization
- **Integrated System**: All auton testing functionality is automatically registered

This system allows you to quickly test and iterate on autonomous routines with precise step-by-step control.