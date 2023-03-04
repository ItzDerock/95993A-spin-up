#include "../config/config.hpp"
#include "../utils/utils.hpp"
#include "./odom.hpp"
#include "lemlib/pid.hpp"
#include "main.h"

void odom::moveTo(XY point, int timeout, float maxSpeed) {
  // state
  float prevLateralPower = 0;
  float prevAngularPower = 0;
  bool close = false;
  int start = pros::millis();
  std::uint8_t compState = pros::competition::get_status();

  // create the PID controller
  lemlib::FAPID lateralPID(0, 0, lateralController.kP, 0, lateralController.kD,
                           "lateralPID");
  lemlib::FAPID angularPID(0, 0, angularController.kP, 0, angularController.kD,
                           "angularPID");

  // set the exit condition
  lateralPID.setExit(lateralController.largeError, lateralController.smallError,
                     lateralController.largeErrorTimeout,
                     lateralController.smallErrorTimeout, timeout);

  // PID loop
  while (pros::competition::get_status() == compState &&
         (!lateralPID.settled() || pros::millis() - start < 300)) {
    // get the current pose
    auto pose = odom::getState();
    pose.theta = fmod(pose.theta, 360);

    // update error
    float deltaX = point.x - pose.x;
    float deltaY = point.y - pose.y;
    float targetTheta =
        fmod(lemlib::radToDeg(M_PI_2 - atan2(deltaY, deltaX)), 360);
    float hypot = std::hypot(deltaX, deltaY);
    float diffTheta1 = lemlib::angleError(pose.theta, targetTheta);
    float diffTheta2 = lemlib::angleError(pose.theta, targetTheta + 180);
    float angularError = (std::fabs(diffTheta1) < std::fabs(diffTheta2))
                             ? diffTheta1
                             : diffTheta2;
    float lateralError = hypot * cos(lemlib::degToRad(std::fabs(diffTheta1)));

    // calculate speed
    float lateralPower = lateralPID.update(lateralError, 0);
    float angularPower = -angularPID.update(angularError, 0);

    // if the robot is close to the target
    if (utils::getDistance({pose.x, pose.y}, point) < 7.5) {
      close = true;
      maxSpeed =
          (std::fabs(prevLateralPower) < 30) ? 30 : std::fabs(prevLateralPower);
    }

    // limit acceleration
    if (!close)
      lateralPower =
          lemlib::slew(lateralPower, prevLateralPower, lateralController.slew);
    if (std::fabs(angularError) > 25)
      angularPower =
          lemlib::slew(angularPower, prevAngularPower, angularController.slew);

    // cap the speed
    if (lateralPower > maxSpeed)
      lateralPower = maxSpeed;
    else if (lateralPower < -maxSpeed)
      lateralPower = -maxSpeed;
    if (close)
      angularPower = 0;

    prevLateralPower = lateralPower;
    prevAngularPower = angularPower;

    float leftPower = lateralPower + angularPower;
    float rightPower = lateralPower - angularPower;

    // ratio the speeds to respect the max speed
    float ratio =
        std::max(std::fabs(leftPower), std::fabs(rightPower)) / maxSpeed;
    if (ratio > 1) {
      leftPower /= ratio;
      rightPower /= ratio;
    }

    // move the motors
    drivetrain.leftMotors->move(leftPower);
    drivetrain.rightMotors->move(rightPower);

    // delay
    pros::delay(10);
  }

  // stop the motors
  drivetrain.leftMotors->move(0);
  drivetrain.rightMotors->move(0);
}
