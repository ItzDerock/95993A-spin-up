#include "auton.hpp"
#include "../config/config.hpp"
#include "../odom/odom.hpp"
#include "lemlib/util.hpp"

// number of times to rotate a roller to change color
const double INTAKE_MOVE_ROTS = 5.5;

// selected auton
auton::AutonMode auton::autonMode = auton::NONE;

// set auton mode
void auton::setAutonMode(AutonMode mode) { autonMode = mode; }

// position
void auton::resetPosition() {
  odom::XYTheta startState = {0, 0, 0};

  switch (autonMode) {
  case auton::FAR_ROLLER:
    startState = {-14, 58, lemlib::degToRad(180)};
    break;

  case auton::CLOSE_ROLLER:
  case auton::SKILLS:
    startState = {56, -37, lemlib::degToRad(90)};
    break;

  case auton::NONE:
  default:
    break;
  }

  odom::reset(startState);
}

// auton modes
void auton::closeRoller() {
  // forward and grab roller
  drivetrain.leftMotors->move_velocity(50);
  drivetrain.rightMotors->move_velocity(50);
  pros::delay(500);
  intake->move_relative(INTAKE_MOVE_ROTS * 360, 200);
  pros::delay(500);
  drivetrain.leftMotors->move_velocity(0);

  // back up a little
  drivetrain.leftMotors->move_velocity(-50);
  drivetrain.rightMotors->move_velocity(-50);
  pros::delay(500);
  drivetrain.leftMotors->move_velocity(0);
  drivetrain.rightMotors->move_velocity(0);

  // start intaking
  intake->move_velocity(200);

  // move and shoot
  odom::moveTo({24, -47}, 15000, 150);
}
