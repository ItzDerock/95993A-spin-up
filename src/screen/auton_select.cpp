#include "../autonomous/auton.hpp"
#include "../config/config.hpp"
#include "main.h"
#include "screen.hpp"

// create the menu
lv_obj_t *autonMenu = lv_ddlist_create(lv_scr_act(), NULL);
lv_style_t style_list;

// reduce the padding to 3 px

// initailize the menu
void display::initializeAutonSelect() {
  // set the options
  lv_ddlist_set_options(autonMenu, "Close Roller\nFar Roller\nSkills\nNone");

  // set the position
  lv_obj_set_pos(autonMenu, 200, 0);

  // set the size
  lv_obj_set_size(autonMenu, 200, 50);

  // reduce the padding to 3 px
  lv_style_copy(&style_list, &lv_style_plain);
  style_list.body.padding.hor = 3;
  style_list.body.padding.ver = 3;
  style_list.body.padding.inner = 3;
  lv_ddlist_set_style(autonMenu, LV_DDLIST_STYLE_BG, &style_list);

  // create the task
  Task autonMenuTask(display::autonSelectTask);
}

// task
void display::autonSelectTask() {
  while (true) {
    // get the selected option
    int selected = lv_ddlist_get_selected(autonMenu);

    // set the auton - corresponds to the enum's value
    auton::setAutonMode((auton::AutonMode)selected);

    /// wait
    delay(20);
  }
}
