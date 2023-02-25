#include "../config/config.hpp"
#include "main.h"
#include "screen.hpp"

// displays a menu for selecting autonomous programs

// list of autonomous programs
const char *autonNames[] = {"Close Roller", "Far Roller", "Skills", "None"};

// the current autonomous program
int autonSelection = 0;
