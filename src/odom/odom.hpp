#pragma once
#include "main.h"
#include "okapi/api/units/QLength.hpp"

namespace odom {

// configuration
extern const double TICKS_PER_CM;

// point structure
struct Point {
  double x, y, angle;
};

// processed point is in a unit that can be understood
struct ProcessedPoint {
  QLength x, y;
  double angle;
};

// the global position of the robot in an unspecified unit
extern odom::Point globalPosition;

// the processed global position of the robot
extern odom::ProcessedPoint position;

// other variables to help calculate the global position
extern odom::Point previousPosition;
extern odom::Point globalDeltaPoint;
extern odom::Point localDeltaPoint;

// odometry functions
void updateSensors();
void updatePosition();
void updateProcessedPosition();
void resetPosition();
void run();

} // namespace odom
