#include "../config/config.hpp"
#include "../pid/pid.hpp"
#include "../structs/point.hpp"
#include "../utils/utils.hpp"
#include "main.h"
#include "tasks.hpp"

// constants
XYPoint tasks::autoAimTarget = {53, 53};

// configuration
bool tasks::autoAimEnabled = true;

// target
double tasks::turretTargetAngle = 0;

// pid
PIDController turretPID(0.1, 0, 0.05);

void tasks::aimTurret() {
  while (true) {
    // get the current location
    auto state = chassis->getPose();

    // calculate angle to target
    tasks::turretTargetAngle = atan2(tasks::autoAimTarget.y - state.y,
                                     tasks::autoAimTarget.x - state.x);

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
