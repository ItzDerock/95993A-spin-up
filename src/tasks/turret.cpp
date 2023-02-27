#include "../config/config.hpp"
#include "../pid/pid.hpp"
#include "../utils/utils.hpp"
#include "main.h"
#include "tasks.hpp"

// constants
const double targetX = 53;
const double targetY = 53;

// target
double tasks::turretTargetAngle = 0;

// pid
PIDController turretPID(0.1, 0, 0.05);

void tasks::aimTurret() {
  while (true) {
    // get the current location
    auto state = chassis->getPose();

    // calculate angle to target
    tasks::turretTargetAngle = atan2(targetY - state.y, targetX - state.x);

    // turn into degrees
    tasks::turretTargetAngle = lemlib::radToDeg(tasks::turretTargetAngle);

    // calculate the error
    double error = tasks::turretTargetAngle - turret_rot->get_angle();

    // calculate the output
    double output = turretPID.update(error);

    // set the output
    turret->move(output);

    // wait
    delay(10);
  }
}
