#include "../config/config.hpp"
#include "../odom/odom.hpp"
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
PIDController turretPID(0.8, 0, 0.5);

void tasks::aimTurret() {
  while (true) {
    // get the current location
    // auto state = chassis->getPose();
    auto state = odom::getState();

    // calculate angle to target
    tasks::turretTargetAngle = atan2(tasks::autoAimTarget.y - state.y,
                                     tasks::autoAimTarget.x - state.x);

    // turn into degrees
    tasks::turretTargetAngle = lemlib::radToDeg(tasks::turretTargetAngle);

    // calculate the error
    // double error =
    //     tasks::turretTargetAngle - ((double)turret_rot->get_angle() / 100);

    // double error = 270 - state.theta - tasks::turretTargetAngle;

    tasks::turretTargetAngle =
        (90 - tasks::turretTargetAngle) - state.theta + 180;

    double error =
        tasks::turretTargetAngle - ((double)turret_rot->get_angle() / 100);

    printf("turret angle: %d\n", turret_rot->get_angle());
    printf("turret target angle: %f\n", tasks::turretTargetAngle);
    printf("error: %f\n", error);

    // error -= state.theta;

    // normalize the error
    if (error > 180) {
      error -= 360;
    } else if (error < -180) {
      error += 360;
    }

    // print the error
    printf("errorCalc: %f\n", error);

    // if error > 45 deg or < 45 deg, out of range for turret
    if (error > 40 || error < -40) {
      // set the output to 0
      turret->move_velocity(0);
      // wait
      delay(10);
      // continue
      continue;
    }

    // calculate the output
    double output = turretPID.update(error);

    // print
    printf("output: %f\n", output);

    // set the output
    turret->move_velocity(output);

    // wait
    delay(10);
  }
}
