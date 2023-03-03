#include "auton.hpp"
#include "../config/config.hpp"
#include "../odom/odom.hpp"

// selected auton
auton::AutonMode auton::autonMode = auton::NONE;

// set auton mode
void auton::setAutonMode(AutonMode mode) { autonMode = mode; }

// position
void auton::resetPosition() { odom::reset(); }
