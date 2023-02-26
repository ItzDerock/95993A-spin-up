#include "auton.hpp"

// selected auton
auton::AutonMode auton::autonMode = auton::NONE;

// set auton mode
void auton::setAutonMode(AutonMode mode) { autonMode = mode; }
