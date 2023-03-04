#pragma once
#include "main.h"

namespace odom {

struct XY {
  double x, y;
};

struct XYTheta {
  double x, y, theta;
};

void update();
void init();
void reset();
void reset(XYTheta startState);
XYTheta getState();
XYTheta getState(bool radians);

// movement functions
void moveTo(XY point, int timeout, float maxSpeed);

} // namespace odom
