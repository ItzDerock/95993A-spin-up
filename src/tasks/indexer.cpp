#include "../config/config.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "tasks.hpp"

Task tasks::indexOne() {
  return Task([]() {
    // set to HIGH
    indexer->set_value(1);

    // wait 0.5 second
    delay(500);

    // set to LOW
    indexer->set_value(0);
  });
}
