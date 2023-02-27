#include "auton.hpp"
#include "../config/config.hpp"

// selected auton
auton::AutonMode auton::autonMode = auton::NONE;

// set auton mode
void auton::setAutonMode(AutonMode mode) { autonMode = mode; }

// position
void auton::resetPosition() { chassis->setPose({0, 0, 0}); }
