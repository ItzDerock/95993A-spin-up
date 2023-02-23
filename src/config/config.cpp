// motors and all other shared stuff go here
#include "./config.hpp"
#include "main.h"
#include "okapi/api/util/mathUtil.hpp"

// inertial sensor
std::shared_ptr<pros::IMU> inertial =
    std::make_shared<pros::IMU>(INERTIAL_PORT);

// odometry
std::shared_ptr<ADIEncoder> odom_middle =
    std::make_shared<ADIEncoder>(ODOM_MIDDLE_1, ODOM_MIDDLE_2, false);

std::shared_ptr<ADIEncoder> odom_left =
    std::make_shared<ADIEncoder>(ODOM_LEFT_1, ODOM_LEFT_2, false);

std::shared_ptr<ADIEncoder> odom_right =
    std::make_shared<ADIEncoder>(ODOM_RIGHT_1, ODOM_RIGHT_2, true);

// create the motors
std::shared_ptr<okapi::Motor> drive_top_left = std::make_shared<okapi::Motor>(
    DRIVE_TOP_LEFT, true, AbstractMotor::gearset::green,
    AbstractMotor::encoderUnits::degrees);

std::shared_ptr<okapi::Motor> drive_top_right = std::make_shared<okapi::Motor>(
    DRIVE_TOP_RIGHT, false, AbstractMotor::gearset::green,
    AbstractMotor::encoderUnits::degrees);

std::shared_ptr<okapi::Motor> drive_bottom_right =
    std::make_shared<okapi::Motor>(DRIVE_BOTTOM_RIGHT, false,
                                   AbstractMotor::gearset::green,
                                   AbstractMotor::encoderUnits::degrees);

std::shared_ptr<okapi::Motor> drive_bottom_left =
    std::make_shared<okapi::Motor>(DRIVE_BOTTOM_LEFT, true,
                                   AbstractMotor::gearset::green,
                                   AbstractMotor::encoderUnits::degrees);

// turret
std::shared_ptr<okapi::Motor> turret = std::make_shared<okapi::Motor>(
    TURRET_PORT, false, AbstractMotor::gearset::red,
    AbstractMotor::encoderUnits::degrees);

// flywheel
std::shared_ptr<okapi::Motor> flywheel = std::make_shared<okapi::Motor>(
    FLYWHEEL_PORT, false, AbstractMotor::gearset::blue,
    AbstractMotor::encoderUnits::degrees);

// create a chassis
std::shared_ptr<OdomChassisController> chassis =
    ChassisControllerBuilder()
        .withMotors({drive_top_left, drive_bottom_left},
                    {drive_top_right, drive_bottom_right})
        .withDimensions({AbstractMotor::gearset::green, (84.0 / 36.0)},
                        {{4_in, 14.8_in}, imev5GreenTPR})
        .withSensors(odom_left, odom_right, odom_middle)
        .withOdometry({{2.75_in, 7_in}, quadEncoderTPR})
        .withGains({0.000425, 0, 0.00001}, // distance
                   {0.000425, 0, 0.0001},  // angle
                   {0.0006, 0, 0.0001})    // turn
        .buildOdometry();
