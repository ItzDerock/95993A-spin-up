#include "./odom.hpp"
#include "../config/config.hpp"
#include "main.h"

// Task to update the odom
Task *odomTask = nullptr;

struct {
  double left, right, center, theta;
} prevSensors = {0, 0, 0, 0};

odom::XYTheta state = {0, 0, 0};

void odom::update() {
  // // skip runs when all sensors are not initialized
  // if (sensors.horizontal1 == nullptr || sensors.horizontal2 == nullptr ||
  //     sensors.vertical1 == nullptr) {
  //   // printf("Skipping odom update because sensors are not initialized");
  //   return;
  // }

  // 1. Store the current encoder values
  auto left = left_tracking_wheel.getDistanceTraveled();
  auto right = right_tracking_wheel.getDistanceTraveled();
  auto center = middle_tracking_wheel.getDistanceTraveled();

  // 2. Calculate delta values
  auto dL = left - prevSensors.left;
  auto dR = right - prevSensors.right;
  auto dC = center - prevSensors.center;

  // 3. Update the previous values
  prevSensors.left = left;
  prevSensors.right = right;
  prevSensors.center = center;

  // 4. (skipped)
  // 5. Calculate new orientation
  auto newTheta = (dL - dR) / (left_tracking_wheel.getOffset() +
                               right_tracking_wheel.getOffset());

  // 6. Calculate change in orientation
  auto dTheta = newTheta - prevSensors.theta;

  // 7. Calculate local offset for dTheta = 0
  odom::XY localOffset = {0, 0};

  if (dTheta == 0) {
    localOffset.x = dC;
    localOffset.y = dR;
  } else {
    // 8. Otherwise, calculate local offset with formula.
    localOffset.x = 2 * sin(newTheta / 2) *
                    (dC / dTheta + (middle_tracking_wheel.getOffset()));
    localOffset.y = 2 * sin(newTheta / 2) *
                    (dR / dTheta + (right_tracking_wheel.getOffset()));
  }

  // 9. Calculate the average orientation
  auto thetam = prevSensors.theta + dTheta / 2;

  // 10. Calculate the global offset
  odom::XY globalOffset = {0, 0};

  globalOffset.x = localOffset.x * cos(thetam) - localOffset.y * sin(thetam);
  globalOffset.y = localOffset.x * sin(thetam) + localOffset.y * cos(thetam);

  // 11. Update the global position
  state.x += globalOffset.x;
  state.y += globalOffset.y;
  state.theta += dTheta;
}

void odom::init() {
  if (odomTask == nullptr) {
    odomTask = new Task([]() {
      while (true) {
        update();
        pros::delay(10);
      }
    });
  }
}

void odom::reset() {
  // stop task
  if (odomTask != nullptr) {
    odomTask->remove();
    odomTask = nullptr;
  }

  // reset encoders
  left_tracking_wheel.reset();
  right_tracking_wheel.reset();
  middle_tracking_wheel.reset();

  // reset state
  state = {0, 0, 0};

  // reset prevSensors
  prevSensors = {0, 0, 0, 0};

  // restart task
  init();
}

odom::XYTheta odom::getState(bool radians) {
  if (radians) {
    return state;
  } else {
    return {state.x, state.y, state.theta * (180 / M_PI)};
  }
}

odom::XYTheta odom::getState() {
  // default to false
  return getState(false);
}
