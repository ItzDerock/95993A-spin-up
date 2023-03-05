# 95993A's Vex Spin Up spectacular code
Code behind 95993A's turret.  

Has the following features:
- Tank drive controls
- Odometry
  - 3 tracking wheels
- Pure Persuit for autonomous (not fully implemented yet)
- LVGL powered display
  - Displays a map of the field with odom data.
- Auto-aiming for the turret
  - Some math with arcsin feeding into a PID loop. 

Built using `C++` and Purdue's PROS library for Vex.  
Also uses LemLib library for structures.

**Competition Update:** Won the Think Award at state, but unfortunately, this does not qualify for worlds
![cool award](https://derock.media/r/XHdnET.jpg)

## File Structure
### `src/`
Holds all of our code

### `src/config/*`
The robot configuration options like port numbers, etc.  

### `src/pid/*`
A PID (Proportional, Integral, Derivative) controller implemented as a class.

### `src/odom/*`
3-wheel odometry system based on the [Pilon's (5225) Arc-based position tracking paper](http://thepilons.ca/wp-content/uploads/2018/10/Tracking.pdf)

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
