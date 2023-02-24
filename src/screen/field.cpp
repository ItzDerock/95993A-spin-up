#include "../config/config.hpp"
#include "display/lv_objx/lv_img.h"
#include "screen.hpp"

// draws the field as an image
// then draws a dot representign the robot's position
// uses lvgl
LV_IMG_DECLARE(field);

// create the image
void display::initializeField() {
  lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(img, &field);
  lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
}
