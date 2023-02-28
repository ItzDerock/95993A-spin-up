#include "../config/config.hpp"
#include "main.h"
#include "tasks.hpp"

void tasks::controllerInfo() {
  while (true) {
    // get flywheel rpm
    double rpm = flywheel_left->get_actual_velocity();

    // get the target rpm
    double targetRPM = tasks::flywheelTargetSpeed;

    // depending on the range, text = LOW/MED/HIGH
    std::string target;

    if (targetRPM < 200) {
      target = "LOW";
    } else if (targetRPM < 400) {
      target = "MED";
    } else {
      target = "HIGH";
    }

    // get autoaim status
    std::string autoaim = tasks::autoAimEnabled ? "ON" : "OFF";

    // build a big string
    std::string info =
        std::to_string((int)rpm) + "rpm | " + target + " | " + "AA: " + autoaim;

    // set the screen
    partner.set_text(0, 0, info.c_str());

    // wait, controller is slow at updating
    pros::delay(100);
  }
}
