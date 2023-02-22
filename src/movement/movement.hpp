#pragma once
#include "../pid/pid.hpp"
#include "main.h"
#include "okapi/api/units/QLength.hpp"

namespace movement {

// variables
extern pros::Task *pidTask;
extern PIDController lateralPIDController;
extern PIDController turnController;
extern bool isSettled;
extern QLength maxError;
extern double maxTurnError;
extern double maxVelocity;

// target type
enum TargetType { POINT, ANGLE, NONE };
struct Target {
  QLength targetX;
  QLength targetY;
  double targetAngle;
  TargetType targetType;
};

// the target
extern Target target;

// manage the background PID task
void startPID();
void stopPID();

// utility functions
void waitUntilSettled();

// movement functions
void turnToAngle(double angle);
void driveDistance(QLength distance);

// management functions
void setMaxVelocity(double maxVelocity);
void setMaxError(QLength maxError);
void setMaxTurnError(double maxError);

} // namespace movement
