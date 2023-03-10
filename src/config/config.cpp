// motors and all other shared stuff go here
#include "./config.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "okapi/api/util/mathUtil.hpp"
#include "pros/adi.hpp"
#include "pros/motors.h"
#include <memory>

// controller
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller partner(pros::E_CONTROLLER_PARTNER);

// turret
std::shared_ptr<pros::Motor> turret =
    std::make_shared<pros::Motor>(TURRET_PORT, pros::E_MOTOR_GEARSET_36, false,
                                  pros::E_MOTOR_ENCODER_DEGREES);

// rotational sensor
std::shared_ptr<Rotation> turret_rot =
    std::make_shared<Rotation>(TURRET_ROT_SENSOR, true);

// flywheel
std::shared_ptr<Motor> flywheel_left =
    std::make_shared<Motor>(FLYWHEEL_LEFT, pros::E_MOTOR_GEARSET_18, true,
                            pros::E_MOTOR_ENCODER_DEGREES);
std::shared_ptr<Motor> flywheel_right =
    std::make_shared<Motor>(FLYWHEEL_RIGHT, pros::E_MOTOR_GEARSET_18, false,
                            pros::E_MOTOR_ENCODER_DEGREES);
MotorGroup flywheel({*flywheel_left, *flywheel_right});

// indexer
std::shared_ptr<pros::ADIDigitalOut> indexer =
    std::make_shared<pros::ADIDigitalOut>(
        std::make_pair(THREEWIRE_EXPANDER, INDEXER_PORT));

// endgame
std::shared_ptr<pros::ADIDigitalOut> endgame =
    std::make_shared<pros::ADIDigitalOut>(ENDGAME_PORT);

// intake
std::shared_ptr<pros::Motor> intake =
    std::make_shared<pros::Motor>(INTAKE_PORT, pros::E_MOTOR_GEARSET_36, false,
                                  pros::E_MOTOR_ENCODER_DEGREES);

// inertial sensor
IMU inertial(INERTIAL_PORT);

// odometry
ADIEncoder odom_middle({THREEWIRE_EXPANDER, ODOM_MIDDLE_1, ODOM_MIDDLE_2},
                       false);
ADIEncoder odom_left({THREEWIRE_EXPANDER, ODOM_LEFT_1, ODOM_LEFT_2}, false);
ADIEncoder odom_right({THREEWIRE_EXPANDER, ODOM_RIGHT_1, ODOM_RIGHT_2}, true);

// create the motors
std::shared_ptr<Motor> drive_top_left =
    std::make_shared<Motor>(DRIVE_TOP_LEFT, E_MOTOR_GEARSET_18, true);

std::shared_ptr<Motor> drive_top_right =
    std::make_shared<Motor>(DRIVE_TOP_RIGHT, E_MOTOR_GEARSET_18, false);

std::shared_ptr<Motor> drive_bottom_left =
    std::make_shared<Motor>(DRIVE_BOTTOM_LEFT, E_MOTOR_GEARSET_18, true);

std::shared_ptr<Motor> drive_bottom_right =
    std::make_shared<Motor>(DRIVE_BOTTOM_RIGHT, E_MOTOR_GEARSET_18, false);

// make motor groups
MotorGroup drive_left({*drive_top_left, *drive_bottom_left});
MotorGroup drive_right({*drive_top_right, *drive_bottom_right});

// create a drivetrain
lemlib::Drivetrain_t drivetrain{&drive_left, &drive_right, 14.75, 4, 476};

// build the odometry setup
lemlib::TrackingWheel left_tracking_wheel(&odom_left, 2.75, 3.5);
lemlib::TrackingWheel right_tracking_wheel(&odom_right, 2.75, 3.5);
lemlib::TrackingWheel middle_tracking_wheel(&odom_middle, 2.75, 3.5);
lemlib::OdomSensors_t sensors{&left_tracking_wheel, &right_tracking_wheel,
                              &middle_tracking_wheel, nullptr, &inertial};

// create the pid system
lemlib::ChassisController_t lateralController{
    20,  // kP
    30,  // kD
    1,   // smallErrorRange
    100, // smallErrorTimeout
    3,   // largeErrorRange
    500, // largeErrorTimeout
    5    // slew rate
};

// turning PID
lemlib::ChassisController_t angularController{
    2,   // kP
    10,  // kD
    1,   // smallErrorRange
    100, // smallErrorTimeout
    3,   // largeErrorRange
    500, // largeErrorTimeout
    5    // slew rate
};

// create the chassis
std::shared_ptr<lemlib::Chassis> chassis = std::make_shared<lemlib::Chassis>(
    drivetrain, lateralController, angularController, sensors);
