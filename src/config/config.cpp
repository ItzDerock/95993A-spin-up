// motors and all other shared stuff go here
#include "./config.hpp"
#include "main.h"

// inertial sensor
std::shared_ptr<pros::IMU> inertial =
    std::make_shared<pros::IMU>(INERTIAL_PORT);

// odometry
std::shared_ptr<ADIEncoder> odom_middle =
    std::make_shared<ADIEncoder>(ODOM_MIDDLE_1, ODOM_MIDDLE_2, false);

std::shared_ptr<ADIEncoder> odom_left =
    std::make_shared<ADIEncoder>(ODOM_LEFT_1, ODOM_LEFT_2, false);

std::shared_ptr<ADIEncoder> odom_right =
    std::make_shared<ADIEncoder>(ODOM_RIGHT_1, ODOM_RIGHT_2, false);

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

// create a chassis
std::shared_ptr<ChassisController> chassis =
    ChassisControllerBuilder()
        .withMotors({drive_top_left, drive_bottom_left},
                    {drive_top_right, drive_bottom_right})
        .withDimensions({AbstractMotor::gearset::green, (60.0 / 36.0)},
                        {{4_in, 11.5_in}, imev5GreenTPR})
        .build();