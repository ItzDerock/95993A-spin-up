#include "../config/config.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "tasks.hpp"

Task tasks::indexOne() {
  return Task([]() {
    indexer->set_value(0);
    // wait 0.5 second
    delay(500);
    indexer->set_value(1);
  });
}
