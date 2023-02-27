#include "main.h"
#include "./config/config.hpp"
#include "./tasks/tasks.hpp"
#include "./utils/utils.hpp"
#include "pros/misc.h"
#include "screen/screen.hpp"

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
  // init auton selector
  display::initializeAutonSelect();

  // initalize odometry
  chassis->calibrate();

  // start turret aiming
  Task aimTurretTask(tasks::aimTurret);

  // Task printOdomTask(printOdom);
  // display initialization
  display::initializeField();
  display::initializeInformation();
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
void autonomous() { chassis->moveTo(0, 10, 5000); }

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
  while (true) {
    // Update drivetrain motors
    int left = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int right = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

    // map to the right speed
    left = utils::mapValue(0, -127, 127, -200, 200);
    right = utils::mapValue(0, -127, 127, -200, 200);

    // set the motors
    drivetrain.leftMotors->move_velocity(left);
    drivetrain.rightMotors->move_velocity(right);

    // wait
    pros::delay(10);
  }
}
