#include "../config/config.hpp"
#include "tasks.hpp"

// displays the flywheel temp on the controller
void tasks::flywheelTemperatureTask() {
  while (true) {
    // master.set_text(0, 0,
    //                 std::to_string((int)flywheel->get_temperature()) + "° | "
    //                 +
    //                     std::to_string((int)flywheel->get_actual_velocity())
    //                     + "rpm" + " | " +
    //                     std::to_string((int)flywheel->get_power()) + "W");

    pros::delay(1000);
  }
}
