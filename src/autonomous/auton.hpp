#pragma once

namespace auton {

enum AutonMode { CLOSE_ROLLER, FAR_ROLLER, SKILLS, NONE };
extern AutonMode autonMode;

// set auton mode
void setAutonMode(AutonMode mode);
void resetPosition();

// run the selected auton
void run();

// for the differnet auton modes
void closeRoller();
void farRoller();
void skills();

} // namespace auton
