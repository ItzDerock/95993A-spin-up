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
XYTheta getState();
XYTheta getState(bool radians);
} // namespace odom
