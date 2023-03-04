#include "../config/config.hpp"
#include "main.h"
#include "tasks.hpp"
#include <cmath>

// target speed
double tasks::flywheelTargetSpeed = 0;
bool tasks::flywheelEnabled = false;

// automatically set the flywheel speed
// function that models the speed required given the distance
double tasks::getFlywheelSpeed(double distance) {
  if (distance < 57) {
    return 110;
  } else {
    return 200;
  }
}

// the task that will update the speed
void tasks::flywheelSpeed() {
  while (true) {
    // set the speed based on distance only if autoaim is on
    if (tasks::autoAimEnabled) {
      // get the current location
      auto state = chassis->getPose();

      // calculate distance to target
      double dist = hypot(tasks::autoAimTarget.y - state.y,
                          tasks::autoAimTarget.x - state.x);

      // calculate the speed
      tasks::flywheelTargetSpeed = tasks::getFlywheelSpeed(dist);
    }

    // set the speed
    flywheel.move(tasks::flywheelEnabled ? tasks::flywheelTargetSpeed : 0);

    // delay
    delay(10);
  }
}
