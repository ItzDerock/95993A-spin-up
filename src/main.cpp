#include "main.h"
#include "./config/config.hpp"
#include "./tasks/tasks.hpp"
#include "./utils/utils.hpp"
#include "odom/odom.hpp"
#include "pros/misc.h"
#include "screen/screen.hpp"

// void printOdom() {
//   while (1) {
//     auto pose = chassis->getPose();
//     printf("x: %f, y: %f, angle: %f\n", pose.x, pose.y, pose.theta);
//     pros::delay(50);
//   }
// }

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
  // chassis->calibrate();
  odom::init();

  // start turret aiming
  Task aimTurretTask(tasks::aimTurret);
  Task flywheelSpeedTask(tasks::flywheelSpeed);

  // set indexer to low position
  indexer->set_value(1);

  // set endgame to low position
  // endgame->set_value(1);

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
    left = utils::mapValue(left, -127, 127, -200, 200);
    right = utils::mapValue(right, -127, 127, -200, 200);

    // set the motors
    drivetrain.leftMotors->move_velocity(left);
    drivetrain.rightMotors->move_velocity(right);

    /**
     * DEROCK's CONTROLS
     */
    // toggle intake
    if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_R1)) {
      if (intake->get_target_velocity() == -200 ||
          intake->get_target_velocity() == 0) {
        intake->move_velocity(200);
      } else {
        intake->move_velocity(0);
      }
    }

    if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_R2)) {
      if (intake->get_target_velocity() == 200 ||
          intake->get_target_velocity() == 0) {
        intake->move_velocity(-200);
      } else {
        intake->move_velocity(0);
      }
    }

    /*
     * SHAAN's CONTROLS (controller2)
     */

    // turret/flywheel requires autoaim off
    if (!tasks::autoAimEnabled) {
      // increase/decrease speed
      if (partner.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
        tasks::flywheelTargetSpeed += 10;
      } else if (partner.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)) {
        tasks::flywheelTargetSpeed -= 10;
      }

      // speed cap of 600
      tasks::flywheelTargetSpeed = std::min(tasks::flywheelTargetSpeed, 200.0);
      // minimum of 0
      tasks::flywheelTargetSpeed = std::max(tasks::flywheelTargetSpeed, 0.0);

      // enable/disable
      if (partner.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)) {
        tasks::flywheelEnabled = !tasks::flywheelEnabled;
      }

      // turn turret
      if (partner.get_digital(E_CONTROLLER_DIGITAL_R1)) {
        turret->move_velocity(15);
      } else if (partner.get_digital(E_CONTROLLER_DIGITAL_R2)) {
        turret->move_velocity(-15);
      } else {
        turret->move_velocity(0);
      }
    }

    // index
    if (partner.get_digital(E_CONTROLLER_DIGITAL_L2)) {
      tasks::indexOne();
    }

    // toggle autoaim
    if (partner.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
      tasks::autoAimEnabled = !tasks::autoAimEnabled;
    }

    /**
     * DUAL CONTROLS
     */
    // endgame should fire when both users have A pressed
    if (master.get_digital(E_CONTROLLER_DIGITAL_A) &&
        partner.get_digital(E_CONTROLLER_DIGITAL_A)) {
      tasks::fireEndgame();
    }

    // wait
    pros::delay(10);
  }
}
