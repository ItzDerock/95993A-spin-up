#pragma once
#include "../structs/point.hpp"
#include "pros/rtos.hpp"

namespace tasks {

// shared configuration for {turret,flywheel}.cpp
extern bool autoAimEnabled;
extern XYPoint autoAimTarget;

// turret.cpp - Turret aiming task
extern double turretTargetAngle;
void aimTurret();

// flywheel.cpp - Flywheel control task
extern double flywheelTargetSpeed;
extern bool flywheelEnabled;
void flywheelSpeed();
double getFlywheelSpeed(double distance);

// controllerInfo.cpp - Controller information task
void controllerInfo();

// indexer.cpp - Indexer control task
pros::Task *indexOne();

// endgame.cpp - Endgame control task
void fireEndgame();

} // namespace tasks
