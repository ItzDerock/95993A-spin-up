#include "../tasks/tasks.hpp"
#include "main.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "screen.hpp"

// text label
lv_obj_t *informationTextLabel = lv_label_create(lv_scr_act(), NULL);

void display::initializeInformation() {
  // goes to the right of the field
  lv_obj_set_pos(informationTextLabel, 200, 0);

  // create the task
  Task informationTask(display::informationTask);
}

// task
void display::informationTask() {
  while (true) {
    // get the match mode
    auto status = pros::competition::get_status();

    // to string
    std::string compStatus =
        (status == COMPETITION_DISABLED     ? "Disabled"
         : status == COMPETITION_AUTONOMOUS ? "Autonomous"
         : status == COMPETITION_CONNECTED  ? "Driver Control"
                                            : "Unknown");

    // get the battery level
    auto battery = pros::battery::get_capacity();

    // turret target angle
    auto target = tasks::turretTargetAngle;

    // target speed
    auto speed = tasks::flywheelTargetSpeed;

    // build a big string
    std::string text = "Match Mode: " + compStatus + "\n" +
                       "Battery: " + std::to_string((int)battery) + "%\n" +
                       "Turret Target: " + std::to_string(target) + "\n" +
                       "Flywheel Target: " + std::to_string(speed) + "\n";

    // put the information in the text area
    lv_label_set_text(informationTextLabel, text.c_str());

    // wait
    delay(100);
  }
}
