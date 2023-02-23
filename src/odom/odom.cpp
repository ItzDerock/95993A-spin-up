// Big thanks to PiLons 5225A
// their technical paper about odometry really helped here
// http://thepilons.ca/wp-content/uploads/2018/10/Tracking.pdf

#include "odom.hpp"
#include "../config/config.hpp"
#include "../utils/utils.hpp"
#include "okapi/api/units/QLength.hpp"
#include <cmath>

// constants
const double odom::BACK_ENC_OFFSET = 3.0;
const double odom::TICKS_PER_CM = 13.0;
const double odom::LEFT_ENC_OFFSET = 0.0;
const double odom::RIGHT_ENC_OFFSET = 0.0;

// ticks per inch
const double odom::ENCODER_TPR = 360.0;
const double odom::WHEEL_DIAMETER = 2.75; // inches
const double odom::TICKS_PER_INCH =
    odom::ENCODER_TPR / (odom::WHEEL_DIAMETER * M_PI);

// the processed position of the robot in cm
odom::ProcessedPoint odom::position = {0_cm, 0_cm, 0};

// structure to keep sensor values
struct EncoderData {
  double left, right, back, theta;
};

// keep track of the last sensor values
EncoderData currentEncoderData = {0, 0, 0};
EncoderData lastEncoderData = {0, 0, 0};
EncoderData deltaEncoderData = {0, 0, 0};

// keep track of inertial sensor values
// struct {
//   double previous, delta;
// } inertialSensorData = {0, 0, 0};

// utility functions
QLength odom::ticksToLength(double ticks) {
  return ticks / odom::TICKS_PER_INCH * 1_in;
}

// Odometry Functions
void odom::update() {
  // Get the current encoder data
  double currentLeft = odom_left->get();
  double currentRight = odom_right->get();
  double currentMiddle = odom_middle->get();

  // Calculate the change
  double deltaLeft = currentLeft - lastEncoderData.left;
  double deltaRight = currentRight - lastEncoderData.right;
  double deltaMiddle = currentMiddle - lastEncoderData.back;

  // Update the last encoder data
  lastEncoderData.left = currentLeft;
  lastEncoderData.right = currentRight;
  lastEncoderData.back = currentMiddle;

  // Turn into inches
  deltaLeft = ticksToLength(deltaLeft).convert(inch);
  deltaRight = ticksToLength(deltaRight).convert(inch);
  deltaMiddle = ticksToLength(deltaMiddle).convert(inch);

  // Calculate the total change in left and right encoder vales
  double totalDeltaLeft = ticksToLength(currentLeft).convert(inch);
  double totalDeltaRight = ticksToLength(currentRight).convert(inch);

  // Calculate new absolute orientation
  double currentAngle =
      (totalDeltaLeft - totalDeltaRight) / (LEFT_ENC_OFFSET + RIGHT_ENC_OFFSET);

  // Calculate the delta angle
  double deltaAngle =
      currentAngle - lastEncoderData.theta; // current - previous

  // Update the previous angle
  // lastEncoderData.theta = currentAngle;

  // if no change in angle, moved straight
  if (deltaAngle == 0) { // account for slight inaccuracies
    // straight, so just add the change in left and right
    odom::position.y += deltaLeft * 1_in;
    odom::position.x += deltaMiddle * 1_in;
    return;
  }

  // calculate local offset
  double localOffsetX =
      2 * sin(currentAngle / 2) * (deltaMiddle + BACK_ENC_OFFSET);
  double localOffsetY =
      2 * sin(currentAngle / 2) * (deltaRight + RIGHT_ENC_OFFSET);

  // calculate average orientation
  double thetaM = lastEncoderData.theta + deltaAngle / 2;

  // calculate global offset
  // this can be done by converting cartesian to polar, change the angle, and
  // then convert back.
  double globalOffsetX =
      localOffsetX * cos(thetaM) - localOffsetY * sin(thetaM);
  double globalOffsetY =
      localOffsetX * sin(thetaM) + localOffsetY * cos(thetaM);

  // update the position
  odom::position.x += globalOffsetX * 1_in;
  odom::position.y += globalOffsetY * 1_in;

  // update the angle
  odom::position.angle += deltaAngle;

  // update the last encoder data
  lastEncoderData.theta = currentAngle;
}

// task
void odom::run() {
  while (true) {
    odom::update();
    // print
    printf("x: %f, y: %f, theta: %f\n", odom::position.x.convert(inch),
           odom::position.y.convert(inch), odom::position.angle);
    pros::delay(10);
  }
}
