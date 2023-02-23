#pragma once
#include "main.h"
#include "okapi/api/units/QLength.hpp"

namespace odom {

// configuration
extern const double TICKS_PER_CM;
extern const double LEFT_ENC_OFFSET;  // left to center
extern const double RIGHT_ENC_OFFSET; // right to center
extern const double BACK_ENC_OFFSET;  // back to center

extern const double ENCODER_TPR;    // ticks per revolution
extern const double WHEEL_DIAMETER; // diameter of the tracking wheels
extern const double TICKS_PER_INCH; // ticks per inch (calculated from above)

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
void update();
void run();

// utility functions
QLength ticksToLength(double ticks);

} // namespace odom
