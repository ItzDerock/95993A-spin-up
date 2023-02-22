#include "movement.hpp"
#include "../config/config.hpp"
#include "../odom/odom.hpp"
#include "main.h"
#include "okapi/api/units/QLength.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <cmath>

// the pid task
pros::Task *movement::pidTask;

// create the pid controllers
PIDController movement::lateralPIDController(0.00125, 0.0, 0.0001);
PIDController movement::turnController(0.0001, 0.0, 0.0001);

// max error
QLength movement::maxError = 1_cm;
double movement::maxTurnError = 0.1;

// max velocity
double movement::maxVelocity = 127;

// is settled
bool movement::isSettled = false;

// the target
movement::Target movement::target = {0_in, 0_in, 0, movement::TargetType::NONE};

// internal variables
double leftEncoderOffset = 0;
double rightEncoderOffset = 0;
double targetTicks = 0;

// the pid task
void pidTaskFn() {
  while (true) {
    // if no target, don't do anything
    if (movement::target.targetType == movement::TargetType::NONE) {
      pros::delay(10);
      continue;
    }

    // variables to hold the pid output
    double leftOutput = 0.0;
    double rightOutput = 0.0;

    // check what the current target is
    if (movement::target.targetType == movement::TargetType::POINT) {
      // get the relative encoder ticks
      double leftEncoderTicks = odom_left->get() - leftEncoderOffset;
      double rightEncoderTicks = -odom_right->get() - rightEncoderOffset;

      // calculate the average encoder value
      double averageEncoderValue = (leftEncoderTicks + rightEncoderTicks) / 2;

      // calculate the error
      double error = targetTicks - averageEncoderValue;

      // update the pid controllers
      double lateral = movement::lateralPIDController.update(error);

      // calculate the turn needed
      double turn =
          movement::turnController.update(leftEncoderTicks - rightEncoderTicks);

      // calculate the motor outputs
      leftOutput = lateral + turn;
      rightOutput = lateral - turn;

      // debug print
      // printf("left enc: %f, right enc: %f, target: %f, error: %f, lateral:
      // %f, "
      //        "turn: %f\n",
      //        leftEncoderTicks, rightEncoderTicks, targetTicks, error,
      //        lateral, turn);

    } else if (movement::target.targetType == movement::TargetType::ANGLE) {
      // calculate the angle error
      double angleError = movement::target.targetAngle - odom::position.angle;

      // update the pid controllers
      // kDleftOutput = movement::turnController.update(angleError);
      // turnPIDOutput = movement::turnController.update(angleError);

      // check if error is within the max error
      if (abs(angleError) < movement::maxTurnError) {
        movement::isSettled = true;
      } else {
        movement::isSettled = false;
      }
    }

    // set the motor outputs
    chassis->getModel()->tank(leftOutput, rightOutput);

    // wait for the next iteration
    pros::delay(10);
  }
}

// start the PID loop
void movement::startPID() {
  // check if the task is already running
  if (pidTask != nullptr) {
    return;
  }

  // set the type to none
  target.targetType = TargetType::NONE;

  // start the task
  pidTask = new pros::Task(pidTaskFn);
}

// stop the PID loop
void movement::stopPID() {
  // check if the task is already running
  if (pidTask == nullptr) {
    return;
  }

  // stop the task
  pidTask->remove();
  pidTask = nullptr;
}

// wait until the robot is settled
void movement::waitUntilSettled() {
  // wait until the robot is settled
  while (!isSettled) {
    pros::delay(10);
  }
}

// turn to an angle
void movement::turnToAngle(double angle) {
  // set the target
  target.targetAngle = angle;
  target.targetType = TargetType::ANGLE;
}

// drive to a point
void movement::driveDistance(okapi::QLength distance) {
  // set the target
  target.targetType = TargetType::POINT;

  // turn into encoder ticks
  targetTicks = distance.convert(centimeter) * odom::TICKS_PER_CM;

  // set the offsets
  leftEncoderOffset = odom_left->get();
  rightEncoderOffset = odom_right->get();
}

// set the max velocity
void movement::setMaxVelocity(double maxVelocity) {
  // set the max velocity
  movement::maxVelocity = maxVelocity;
}

// set the max error
void movement::setMaxError(okapi::QLength maxError) {
  // set the max error
  movement::maxError = maxError;
}

// set the max turn error
void movement::setMaxTurnError(double maxTurnError) {
  // set the max turn error
  movement::maxTurnError = maxTurnError;
}
