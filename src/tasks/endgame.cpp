#include "../config/config.hpp"
#include "main.h"
#include "tasks.hpp"

Task *endgameTask = nullptr;

void tasks::fireEndgame() {
  if (endgameTask != nullptr) {
    return;
  }

  endgameTask = new Task([]() {
    // fire the endgame
    endgame->set_value(1);
    // delay(500);
    // endgame->set_value(0);
    // delay(100);
    endgameTask = nullptr;
  });
}
