#include "auton.hpp"
#include "../config/config.hpp"
#include "../odom/odom.hpp"
#include "../tasks/tasks.hpp"
#include "lemlib/util.hpp"

// number of times to rotate a roller to change color
const double INTAKE_MOVE_ROTS = 1;

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

void auton::run() {
  switch (autonMode) {
  case auton::FAR_ROLLER:
    auton::farRoller();
    break;

  case auton::CLOSE_ROLLER:
    auton::closeRoller();
    break;

  case auton::SKILLS:
    auton::skills();
    break;

  case auton::NONE:
  default:
    break;
  }
}

// auton modes
void auton::closeRoller() {
  // forward and grab roller
  drivetrain.leftMotors->move_velocity(50);
  drivetrain.rightMotors->move_velocity(50);
  pros::delay(500);
  intake->move_relative(-1 * INTAKE_MOVE_ROTS * 360, 200);
  pros::delay(500);
  drivetrain.leftMotors->move_velocity(0);

  // back up a little
  drivetrain.leftMotors->move_velocity(-50);
  drivetrain.rightMotors->move_velocity(-50);
  pros::delay(250);
  drivetrain.leftMotors->move_velocity(0);
  drivetrain.rightMotors->move_velocity(0);

  // turn 90 degrees
  drivetrain.leftMotors->move_velocity(-50);
  drivetrain.rightMotors->move_velocity(50);

  while (odom::getState().theta > 14) {
    pros::delay(10);
  }

  drivetrain.leftMotors->move_velocity(0);
  drivetrain.rightMotors->move_velocity(0);

  // forward
  drivetrain.leftMotors->move_velocity(100);
  drivetrain.rightMotors->move_velocity(100);

  pros::delay(3000);

  drivetrain.leftMotors->move_velocity(-100);
  drivetrain.rightMotors->move_velocity(-100);

  pros::delay(1000);

  drivetrain.leftMotors->move_velocity(0);
  drivetrain.rightMotors->move_velocity(0);

  // // start intaking
  // intake->move_velocity(200);
  //
  // // move and shoot
  // auto shootTask = new Task([]() {
  //   // wait for turret to be ready
  //   while (!tasks::turretReady) {
  //     pros::delay(50);
  //   }
  //
  //   // shoot
  //   while (tasks::turretReady) {
  //     tasks::indexOne();
  //     pros::delay(50);
  //   }
  // });
  //
  // // move to position
  // odom::moveTo({24, -47}, 15000, 150);
  //
  // // stop the task
  // shootTask->remove();
}

void auton::farRoller() {
  odom::moveTo({13, -29}, 5000, 100);
  // wait for turret to lock
  while (!tasks::turretReady) {
    pros::delay(50);
  }

  // shoot twice
  tasks::indexOne();
  while (tasks::indexerTask != nullptr) {
    pros::delay(10);
  }
  tasks::indexOne();

  // roller
  odom::moveTo({40, -69}, 5000, 200);
  intake->move_relative(INTAKE_MOVE_ROTS * 360, 200);
}

void auton::skills() { odom::moveTo({10, 0}, 5000, 200); }
