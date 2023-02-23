#include "main.h"
#include "./config/config.hpp"
#include "./utils/utils.hpp"
#include "pros/misc.h"

void printOdom() {
  while (1) {
    auto pose = chassis->getPose();
    printf("x: %f, y: %f, angle: %f\n", pose.x, pose.y, pose.theta);
    pros::delay(50);
  }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // initalize odometry
  chassis->calibrate();
  // chassis->setPose(0, 0, 0);
  Task printOdomTask(printOdom);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis->moveTo(0, 10, 5000);

  // chassis->driveToPoint({1_ft, 0_ft});
  // chassis->waitUntilSettled();
  // pros::delay(1000);
  // chassis->driveToPoint({44_in, 0_ft});
  // chassis->waitUntilSettled();
  // pros::delay(1000);
  // chassis->driveToPoint({0_ft, 0_ft});

  // move forward 1 meter
  // movement::driveDistance(1_ft);
  // movement::waitUntilSettled();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  pros::Controller master(pros::E_CONTROLLER_MASTER);

  while (true) {
    // Update drivetrain motors
    int left = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int right = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    drivetrain.leftMotors->move_velocity(left);
    drivetrain.rightMotors->move_velocity(right);

    // turret
    turret->move_velocity(0);
    HELD(pros::E_CONTROLLER_DIGITAL_L1) { turret->move_velocity(10); }
    else HELD(pros::E_CONTROLLER_DIGITAL_L2) {
      turret->move_velocity(-10);
    }

    // flywheel
    flywheel->move_velocity(0);
    HELD(pros::E_CONTROLLER_DIGITAL_R1) { flywheel->move_velocity(600); }
    else HELD(pros::E_CONTROLLER_DIGITAL_R2) {
      flywheel->move_velocity(-600);
    }

    pros::delay(10);
  }
}
