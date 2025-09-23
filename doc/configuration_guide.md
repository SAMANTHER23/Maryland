# Configuration Guide

This guide provides step-by-step instructions for configuring your VEX V5 robot code using RGB template.

**Important:** The **Drivetrain Configuration** section is mandatory for you to be able to test drive. The other sections are optional and depend on your specific needs.
---

## Drivetrain Configuration *(Required)*

This section is **mandatory** - you must configure your drivetrain for the robot to function properly.

### Step 1: Open robot-config.cpp
Navigate to `src/robot-config.cpp` and locate the motor definitions.

### Step 2: Configure Motor Ports
Find the motor definitions and update the port numbers and motor directions:

```cpp
// Left side motors
motor leftMotor1 = motor(PORT1, ratio6_1, true);
motor leftMotor2 = motor(PORT2, ratio6_1, true);
motor leftMotor3 = motor(PORT3, ratio18_1, false);

// Right side motors
motor rightMotor1 = motor(PORT4, ratio6_1, false);
motor rightMotor2 = motor(PORT5, ratio6_1, false);
motor rightMotor3 = motor(PORT6, ratio18_1, true);

// Inertial sensor
inertial inertial1 = inertial(PORT10);
```

**Action:** Replace `PORT1`, `PORT2`, etc. with your actual motor port numbers. Make sure the *motor directions* are correct. If you have a 4-motor setup, simply assign the `leftMotor3`, `rightMotor3` to unused ports.

### Step 3 (optional): Set Drive Mode
Locate the drive mode setting:

```cpp
int DRIVE_MODE = 0;  // 0 for double arcade, 1 for single arcade, 2 for tank, 3 for mecanum
```

**Action:** Set the drive mode value:
- `0` for **Double Arcade** (right stick for forward/backward, left stick for turning)
- `1` for **Single Arcade** (left stick for both forward/backward and turning)
- `2` for **Tank Drive** (left stick for left side, right stick for right side)
- `3` for **Mecanum Drive** (four-wheel independent control for strafing)

### Step 4 (optional): Configure Wheel Size and Gear Ratio
Find the Drive constructor in `robot-config.cpp` and update the wheel diameter and gear ratio parameters:

```cpp
Drive chassis(
  //Left Motors:
  motor_group(leftMotor1, leftMotor2, leftMotor3),
  //Right Motors:
  motor_group(rightMotor1, rightMotor2, rightMotor3),
  //Inertial Sensor:
  inertial1,
  //wheel diameter:
  2.75,
  //Gear ratio of motor to wheel: if your motor has an 36-tooth gear and your wheel has a 48-tooth gear, this value will be 0.75. For direct drive, this value will be 1.
  0.75
);
```

**Action:** 
- Set the wheel diameter (currently `2.75`) to your actual wheel diameter in inches
- Set the gear ratio (currently `0.75`) to your motor-to-wheel gear ratio:
  - `1.0` for direct drive (motor directly connected to wheel)
  - `0.75` for 36:48 gear ratio (36-tooth motor gear, 48-tooth wheel gear)
  - `2.0` for 2:1 gearbox (2 motor rotations = 1 wheel rotation)
  - `0.5` for 1:2 gearbox (0.5 motor rotations = 1 wheel rotation)

---


### (optional) Step 5: Configure Drive Constants
Find the chassis constants in the `setChassisDefaults()` function in `robot-config.cpp`:

```cpp
// Sets the arcade drive constants for the chassis.
// These constants are used to control the arcade drive of the chassis.
chassis.setArcadeConstants(0.5, 0.5, 0.85);
```

**Available Constants:**
- **kBrake (0.5)**: Controls how quickly the robot stops when joysticks are released
- **kTurnBias (0.5)**: Controls the balance between forward/backward and turning movement
- **kTurnDampingFactor (0.85)**: **Controls turn sensitivity** - lower values make turning slower but more accurate, higher values make turning faster.

**Action:** Adjust these values based on your tuning of the chassis driving behavior. 

## Other Subsystems Configuration

### Step 1: Open robot-config.cpp
Navigate to `src/robot-config.cpp` and locate the `other subsystems` section.

### Step 2: Configure Subsystem Motors
Add the motor definitions for your other subsystems:

```cpp
// Intake motors
motor intake_motor = motor(PORT7, ratio18_1, false);
// Other motors as needed
```

**Action:** Add your motors with correct port numbers and gear ratios.

### Step 3: Set Total Motor Count
Locate the motor count definition:

```cpp
int NUMBER_OF_MOTORS = 8;  // Total number of motors
```

**Action:** Update to match your total number of motors.

**Motor Monitoring System:**
The program automatically monitors motor health and will alert the driver if:
- **Disconnected motors**: Controller vibrates with "---" pattern and displays "X motor is disconnected"
- **Overheated motors**: Controller vibrates with "---" pattern and displays "motor X is Y°C" (default temperature limit: 50°C)

**When alerts occur:**
- Controller will vibrate to get driver's attention
- Error message appears on controller screen
- Check motor connections and temperatures immediately

### Step 4: Create Helper Functions
Add helper functions for your subsystems:

```cpp
void inTake() {
    intakeMotor.spin(forward, 100, percent);
}

void stopRollers() {
    intakeMotor.stop();
}
```

**Action:** Create functions for each subsystem you want to control.

### Step 5: Declare Functions in Header
Open `include/robot-config.h` and add function declarations so that `main.cpp` or `autons.cpp` can call the functions:

```cpp
void inTake();
void stopRollers();
// Add other function declarations
```

---

## Button Control Configuration 
### Step 1: Open main.cpp
Navigate to `src/main.cpp` and locate the button functions.

### Step 2: Create Button Functions
Add button functions for your subsystems:

```cpp
void buttonL1Action() {
    inTake();
    while(controller1.ButtonL1.pressing()) {
        wait(20, msec);
    }
    stopRollers();
}

```

**Action:** Create button functions for each action you want to control.

### Step 3: Map Buttons to Functions
Find the button mapping section in the `setupButtonMapping()` function:

```cpp
controller1.ButtonL1.pressed(buttonL1Action);
```

**Action:** Map your button functions to controller buttons.

---

## Autonomous Routines Configuration

### Step 1: Open autons.cpp
Navigate to `src/autons.cpp` and locate the auton functions.

### Step 2: Create Auton Functions
Add your autonomous routines:

```cpp
void auton1() {
    // Drive forward 24 inches
    chassis.driveDistance(24);
}

void auton2() {
    // Simple forward movement (6V = 50% of max 12V)
    chassis.driveWithVoltage(6, 6);
    wait(1000, msec);
    chassis.stop(brake);
}
```

**Action:** Write your autonomous routines using the drive APIs.

### Step 2.5: Voltage Limits
**Important:** The VEX V5 brain operates on a 12V system. When using voltage-based functions:
- **Maximum voltage**: 12V
- **Recommended range**: 2V to 10V for precise control

### Step 3: Map Auton Functions
Find the `runAutonItem()` function:

```cpp
void runAutonItem() {
    switch (currentAutonSelection) {
    case 0:
        auton1();
        break;
    case 1:
        auton2();
        break;
    case 2:
        auton_skill();
        break;
    }
}
```

**Action:** Add your auton functions to the switch statement.

### Step 4: Add Menu Text
Find the `autonMenuText` array:

```cpp
char const * autonMenuText[] = {
    "auton1",
    "auton2",
    "auton_skill"
};
```

**Action:** Add names for your autonomous routines.

### (Optional) Step 5: Step-by-Step Testing 
For complex autons, you can add step-by-step testing capability:

```cpp
void auton_skill() {
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

**Action:** Use `continueAutonStep()` to enable step-by-step testing of complex autons.

---

## Testing Your Configuration

### Step 1: Build the Project
1. Open the project in VSCode
2. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
3. Type "VEX: Build" and select it
4. Check for any compilation errors

### Step 2: Download to Robot
1. Connect your VEX brain via USB
2. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
3. Type "VEX: Download" and select it
4. Wait for the download to complete

### Step 3: Test Driver Control
1. Run the program on the brain
2. Test the set drive mode:
   - **Double Arcade (Mode 0)**: Use right stick for forward/backward, left stick for turning
   - **Single Arcade (Mode 1)**: Use left stick for both forward/backward and turning
   - **Tank Drive (Mode 2)**: Use left stick for left side motors, right stick for right side motors
   - **Mecanum Drive (Mode 3)**: Use left stick for forward/backward and turning, right stick for strafing
3. Verify motors respond correctly

### (Optional) Step 4: Test Autonomous Routines During Driver Control
1. **Enter Test Mode**: Press the `Right button` within 5 seconds of program startup
2. **Navigate Autons**: Use `Right/Left buttons` to cycle through available autons
3. **Navigate Steps**: Use `Up/Down buttons` to navigate through individual steps (for step-enabled autons)
4. **Run Auton**: Press `A button` to execute the selected auton or current step
5. **View Results**: Check controller screen for execution time and step information


---

## Troubleshooting

### Common Issues:
1. **Motors not responding or in conflict**: Check port numbers and motor directions
2. **Robot drives in wrong direction**: Reverse motor directions in robot-config.cpp
3. **Buttons not working**: Verify button mappings in main.cpp
4. **Auton not running**: Check auton function names in runAutonItem()
5. **Test mode not working**: Ensure you press Right button within 5 seconds of startup
6. **Steps not advancing**: Check that `continueAutonStep()` is called in step-enabled autons
7. **Turn sensitivity too high/low**: Adjust `kTurnDampingFactor` value in `setArcadeConstants()` - higher values increase sensitivity, lower values decrease sensitivity
8. **Controller vibrating with "---" pattern**: Check for disconnected or overheated motors - the system automatically monitors motor health

### Debug Tips:
1. Add `printControllerScreen()` statements
2. Use the controller screen to display debug information
3. Check motor temperatures and connections and directions
