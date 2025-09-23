# RGB BOTS template
From Haverford Robotics Team 169X (The Cavalry) and RGB Bots Team 99009A (the little red pony).

This project provides a lightweight C++ template for VEX V5 robotics. It offers a modular architecture and a simple library for PID driving control and autonomous code structure, allowing teams to quickly get their robot operational and adapt the core functionality to their needs. The built-in test mode allows teams to step through and test all of their autonomous routines during driver control, providing a unique way to debug and iterate without having to rebuild multiple programs or require the field switch. Finally, the included sample web application for controlling the robot in real time on mobile devices adds a fun and interactive dimension to VEX robot beyond V5RC competitions (see [demo video](https://youtube.com/shorts/rn8VEuGWva8)).

This project also includes a [python version](RGB-Template-Python/readme.md) of the code with simplified features. If you are a beginner team you might want to check it out. 

## Target Audience

This library is designed specifically for teams who are getting started with VEX V5 c++ programming and do **NOT** have odometry (tracking wheel) setups. Advanced high school teams often use comprehensive libraries like [JAR-Template](https://github.com/JacksonAreaRobotics/JAR-Template) and [LemLib](https://github.com/LemLib/LemLib) that provide features such as path following and pure pursuit. In contrast, this template focuses on simplicity and ease of use, allowing teams to quickly get their robot operational without the complexity of advanced positioning systems.

## Installation
*   **Prerequisites:** Before you begin, make sure:
    - You have [VSCode with VEX extension](https://kb.vex.com/hc/en-us/categories/360002333191-V5?sc=vs-code-extension) installed.
    - (Optional) You have the [GitHub](https://github.com/) account and extension in VS Code installed.
    - (Optional) If you are using Mac with Apple silicon: [install Rosetta](https://support.apple.com/en-us/102527) to simulate Intel chip if errors show up with VEX extension.
*   **Download and Open the Project:** 
    *   Download [the source code as a ZIP file](https://github.com/ericjiangxiao/RGB_Template/archive/refs/heads/main.zip). Unzip and extract the downloaded files to a local folder.
    *   In VSCode, go to `File > Open Folder` and select the extracted folder.
    *  (Optional) Or follow the instructions to [pull the repository](doc/how_to_clone_and_pull_with_vscode.md) from github using VSCode.
*   **(Optional) If the project cannot build correctly**: 
    *   [Create a new v5 project](https://kb.vex.com/hc/en-us/articles/20146074601748-Creating-VS-Code-Projects-for-V5#:~:text=Select%20the%20'New%20Project'%20button,and%20select%20the%20corresponding%20icon.). Copy the dowloaded `src` and the `include` folders to the new project.

## Project Structure

The project is organized into the following directories:

*   `src/`: Main source code
    *   `main.cpp`: Entry point, competition control, and button mappings
    *   `robot-config.cpp`: Configuration for drivetrain motors, sensors, and other subsystems
    *   `autons.cpp`: Autonomous routines
    *   `rgb-template/`: Library code
*   `include/`: Header files
*   `doc/`: Additional documentation
*   `RGB_web_simple/`: Sample web app



## Configuration

For detailed step-by-step configuration instructions, see [Configuration Guide](doc/configuration_guide.md).

### Robot configuration ([robot-config.cpp](src/robot-config.cpp))

*   **Drivetrain Motors and Sensors:** Define the 6-motor drivetrain motors and inertial sensor, including ports, gear ratios, and motor direction. By default, the port numbers are 1, 2, 3 for the left side, 4, 5, 6 for the right side and 10 for the inertial sensor. 
*   **Drive Mode:** Set `DRIVE_MODE` to `0` for double arcade control, `1` for single arcade control, `2` for tank control, or `3` for mecanum control.
*   **Other Motors and Sensors:** Define your motors and sensors for other subsystems such as intake or lift.
*   **Number of Total Motors:** Set `NUMBER_OF_MOTORS` to total number of motors to allow the program to automatically check for disconnected or overheated motors. 
*   **(optional) Helper Functions:** Write helper functions to control the subsystems and declare those functions in [robot-config.h](include/robot-config.h).
*   **(Optional) Wheel Size and Gear Ratio:**
    *  For correct auton driving distance measurement, find the Drive constructor in `robot-config.cpp` and update the wheel diameter and gear ratio parameters
*   **(Optional) Drive Constants:** If needed, adjust any of constants for the drivetrain in the `setChassisDefaults()` function. For example, adjust the `kTurnDampingFactor` value in `setArcadeConstants()` to control turn sensitivity - lower values make turning less sensitive, higher values make turning more sensitive. 

### Driver Control ([main.cpp](src/main.cpp))

*   **Button Functions:** Write your button functions
*   **Button Bindings:** In the `setupButtonMapping()` function, map event handlers of the buttons to the functions.

## Test Sample Program
- **Build Project and Run Program:**
  - Download the program to the brain and use game controller to select the program slot and run the program.
- **Drive Mode:**
  - Double Arcade Drive (default): Use left stick to turn and right stick to drive forward/backward
  - Single Arcade Drive: Use left stick to turn and drive forward/backward
  - Tank Drive: Use left stick for left side motors, right stick for right side motors  
  - Mecanum Drive: Use left stick for forward/backward and turning, right stick for strafing
  - Change drive mode for different drivers: Press the controller's `Left button` within 5 seconds of program startup to switch modes
- **Automatic Motor Health and Game Time Monitoring**: 
  - The controller will vibrate and display warning messages if any motors are disconnected or overheated (temperature limit: 50°C). Check motor connections and temperatures immediately when alerts occur.
  - The controller will vibrate and display the "end game" message near end game.
- **(Experimental) Control the Robot with Mobile Devices** 
  - Follow step-by-step [setup instructions](RGB_web_simple/README.md) to enable WebSocket Server in VSCode VEX Extension, start the sample web server on your local computer and control the robot program on mobile devices.
  - To disable this feature, simply comment out the line `pollCommandMessages();` in the main loop in `main.cpp`.
  - To extend this feature for more robot commands, edit the [web app](RGB_web_simple/EXPLANATION.md) to send additional messages as well as the `pollCommandMessages` function in [main.cpp](src/main.cpp) to parse additional messages.

## Autonomous Routines ([autons.cpp](src/autons.cpp))

-   **Auton Functions:** Write your autonomous routines as separate functions.
-   **Auton Mappings:** Map auton functions to auton menu items in the `runAutonItem()` function 
-   **Auton Menu Text:** Add the names of your autonomous functions to the `autonMenuText` array to make them shown on the brain's and controller's screen.

- **Run individual auton:**
  - Set default auton: Set the `currentAutonSelection` value in `auton.cpp` and choose "timed run" using the controller.
  - Auton Selection:  Before the autonomous period is enabled by the field switch, press the brain screen to scroll through the list of autons.
- **(Optional) Test all autons during driver control**:
    - Press the controller's `Right button` within 5 seconds of program startup to enter test mode.
    - When in test mode, press the `A button` to run the selected auton or current step.
    - When in test mode, press the `Right/Left buttons` to cycle through the list of autonomous routines and press the `Up/Down buttons` to navigate through individual steps of the current auton.
    - At any time, to abort the auton driving, simply move the joystick.
    - See the complete action flow in [Test Auton Button Flow Explanation](doc/test_auton_buttons.md) and the [demo video](https://youtu.be/W6ql04Aj_xQ).


## Programming Interfaces of the library

### Voltage Limits
- **Important:** The VEX V5 brain operates on a 12V system. All voltage-based functions have a maximum voltage of 12V.
- **Recommended range**: 3V to 10V for precise control.

### Drive APIs ([drive.h](include/rgb-template/drive.h))
The `Drive` class provides a set of APIs to control the robot's movement.

### `driveWithVoltage(float leftVoltage, float rightVoltage)`

This is the most basic drive function. It sets the voltage for the left and right sides of the drivetrain directly. 

**Example:**

```cpp
// Drive forward at half speed for 1 second
chassis.driveWithVoltage(6, 6);
wait(1000, msec);
// rotate slowly for 1 second
chassis.driveWithVoltage(-4, 4);
wait(1000, msec);
chassis.stop(coast);
```

### `turnToHeading(...)`

This API turns the robot to a specific heading using a PID controller.

**Variations:**

1.  `turnToHeading(float heading)`: Turns to the specified heading with default parameters.
2.  `turnToHeading(float heading, float turnMaxVoltage)`: Limits the maximum voltage for a slower, more accurate turn.

**Examples:**

```cpp
// Simple turn to 90 degrees
chassis.turnToHeading(90);

// Slower, more accurate turn
chassis.turnToHeading(-90, 6);
```

### `driveDistance(...)`

This API drives the robot a specific distance using a PID controller, with options for maintaining a heading.

**Variations:**

1.  `driveDistance(float distance)`: Drives the specified distance with default parameters.
2.  `driveDistance(float distance, float driveMaxVoltage)`: Limits the maximum voltage for driving.
3.  `driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage)`: Drives while turning a specific heading (curved drive). **For consistent result, choose a lower headingMaxVoltage less than 6V.**

**Examples:**

```cpp
// Drive forward 24 inches
chassis.driveDistance(24);

// Drive backward 24 inches with a maximum voltage of 8
chassis.driveDistance(-24, 8);

// Drive forward 24 inches while turning to a heading of 45 degrees
chassis.driveDistance(24, 10, 45, 4);
```

### `setHeading(...)`

This API set the robot to a specific heading, e.g. when the auton routine starts.

**Examples:**

```cpp
// set robot intial heading to 90 degrees
chassis.setHeading(90);

```
