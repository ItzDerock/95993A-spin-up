# 95993A's Vex Spin Up spectacular code
Code behind 95993A's turret.  

Has the following features:
- Tank drive controls
- Odometry powered by LemLib
  - 3 tracking wheels
  - inertial sensor
- Pure Persuit for autonomous
- LVGL powered display
  - Displays a map of the field with odom data.
- Auto-aiming for the turret

Built using `C++` and Purdue's PROS library for Vex.  
Also uses LemLib library.

## File Structure
### `src/`
Holds all of our code

### `src/config/*`
The robot configuration options like port numbers, etc.  

### `src/pid/*`
A PID (Proportional, Integral, Derivative) controller implemented as a class.

### `src/screen/*`
All the C++ LVGL code for the widgets on the screen.

### `src/tasks/*`
All of the smaller micro-tasks for the robot.

### `src/utils/*`
Small utility functions that are used throughout the code.

### `src/main.cpp`
Entrypoint file, contains code for auton and opcontrol.

### `include/`
Holds the C++ header files for all the libraries.

### `firmware/`
The firmware archives that contains the library's compiled code.

### `utilities/`
Contains some helpful utility programs.
