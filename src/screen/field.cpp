#include "../config/config.hpp"
#include "display/lv_objx/lv_img.h"
#include "screen.hpp"

// draws the field as an image
// then draws a dot representign the robot's position
LV_IMG_DECLARE(field);
LV_IMG_DECLARE(robot);

// constants
const int IMAGE_WIDTH = 200;
const double REAL_WIDTH = 147.8377757;

// create images
lv_obj_t *fieldImage = lv_img_create(lv_scr_act(), NULL);
lv_obj_t *robotImage = lv_img_create(lv_scr_act(), NULL);

// text label
lv_obj_t *textArea = lv_label_create(lv_scr_act(), NULL);

// create the image
void display::initializeField() {
  // set the image
  lv_img_set_src(fieldImage, &field);
  lv_img_set_src(robotImage, &robot);

  // set the position
  lv_obj_set_pos(fieldImage, 0, 0);

  // text area goes below the field
  lv_obj_set_pos(textArea, 0, IMAGE_WIDTH);

  // set the size
  lv_obj_set_size(fieldImage, IMAGE_WIDTH, IMAGE_WIDTH);
  lv_obj_set_size(robotImage, 15, 15);

  // create the task
  Task fieldTask(display::fieldTask);
}

// utility function to turn inches into pixels
int inchesToPixels(double inches) { return inches / REAL_WIDTH * IMAGE_WIDTH; }

// draws the robot's position on the field
void display::fieldTask() {
  while (true) {
    // get the robot's position
    auto state = chassis->getPose();

    // turn the position into pixels
    int x = inchesToPixels(state.x);
    int y = inchesToPixels(state.y);

    // 0,0 is the center of the image
    // so we need to offset it
    x += IMAGE_WIDTH / 2;
    y += IMAGE_WIDTH / 2;

    // set the position
    lv_obj_set_pos(robotImage, x, y);

    // build text
    std::string text = "x: " + std::to_string(state.x) +
                       " y: " + std::to_string(state.y) +
                       "\nangle: " + std::to_string(state.theta);

    // set the text
    lv_label_set_text(textArea, text.c_str());

    // delay for 100ms (10Hz)
    pros::delay(100);
  }
}
