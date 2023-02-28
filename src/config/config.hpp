#pragma once

#include "main.h"
#include "pros/adi.hpp"

/**
 * Drive train ports
 */
#define DRIVE_TOP_LEFT 1
#define DRIVE_TOP_RIGHT 2
#define DRIVE_BOTTOM_LEFT 3
#define DRIVE_BOTTOM_RIGHT 4

/**
 * Turret
 */
#define TURRET_PORT 5
#define TURRET_ROT_SENSOR 11

/**
 * Flywheel
 */
#define FLYWHEEL_LEFT 6
#define FLYWHEEL_RIGHT 7

/**
 * Intake
 */
#define INTAKE_PORT 8

/**
 * Inertial sensor port
 */
#define INERTIAL_PORT 10

/**
 * Odometry
 */
#define THREEWIRE_EXPANDER 20
#define ODOM_MIDDLE_1 'A'
#define ODOM_MIDDLE_2 'B'
#define ODOM_LEFT_1 'C'
#define ODOM_LEFT_2 'D'
#define ODOM_RIGHT_1 'E'
#define ODOM_RIGHT_2 'F'

/**
 * Indexer
 */
#define INDEXER_PORT 'A'

/**
 * Endgame
 */
#define ENDGAME_PORT 'B'

/**
 * utility macros
 */
// #define BUTTON(x, controller) if (controller.get_digital_new_press(x))
// #define HELD(x) if (master.get_digital(x))

// controller
extern pros::Controller master;
extern pros::Controller partner;

// inertial sensor
extern IMU inertial;

// odometry
extern ADIEncoder odom_middle;
extern ADIEncoder odom_left;
extern ADIEncoder odom_right;

// indexer
extern std::shared_ptr<ADIDigitalOut> indexer;

// endgame
extern std::shared_ptr<ADIDigitalOut> endgame;

// rotational sensor
extern std::shared_ptr<Rotation> turret_rot;

// all the motors
extern std::shared_ptr<Motor> drive_top_left;
extern std::shared_ptr<Motor> drive_top_right;
extern std::shared_ptr<Motor> drive_bottom_right;
extern std::shared_ptr<Motor> drive_bottom_left;
extern std::shared_ptr<Motor> turret;
extern std::shared_ptr<Motor> intake;

// flywheel
extern std::shared_ptr<Motor> flywheel_left;
extern std::shared_ptr<Motor> flywheel_right;
extern MotorGroup flywheel;

// LemLib stuff
extern lemlib::Drivetrain_t drivetrain;
extern lemlib::TrackingWheel left_tracking_wheel;
extern lemlib::TrackingWheel right_tracking_wheel;
extern lemlib::TrackingWheel middle_tracking_wheel;
extern lemlib::OdomSensors_t sensors;
extern lemlib::ChassisController_t lateralController;
extern lemlib::ChassisController_t angularController;
extern std::shared_ptr<lemlib::Chassis> chassis;
