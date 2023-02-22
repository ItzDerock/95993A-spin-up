#pragma once

#include "main.h"

/**
 * Drive train ports
 */
#define DRIVE_TOP_LEFT 1
#define DRIVE_TOP_RIGHT 2
#define DRIVE_BOTTOM_LEFT 3
#define DRIVE_BOTTOM_RIGHT 4

/**
 * Inertial sensor port
 */
#define INERTIAL_PORT 10

/**
 * Odometry
 */
#define ODOM_MIDDLE_1 'A'
#define ODOM_MIDDLE_2 'B'
#define ODOM_LEFT_1 'C'
#define ODOM_LEFT_2 'D'
#define ODOM_RIGHT_1 'E'
#define ODOM_RIGHT_2 'F'

/**
 * utility macros
 */
#define BUTTON(x) if (master.get_digital_new_press(x))
#define HELD(x) if (master.get_digital(x))

// inertial sensor
extern std::shared_ptr<pros::IMU> inertial;

// odometry
extern std::shared_ptr<ADIEncoder> odom_middle;
extern std::shared_ptr<ADIEncoder> odom_left;
extern std::shared_ptr<ADIEncoder> odom_right;

// all the motors
extern std::shared_ptr<okapi::Motor> drive_top_left;
extern std::shared_ptr<okapi::Motor> drive_top_right;
extern std::shared_ptr<okapi::Motor> drive_bottom_right;
extern std::shared_ptr<okapi::Motor> drive_bottom_left;

// create a chassis
extern std::shared_ptr<OdomChassisController> chassis;
