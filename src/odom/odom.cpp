#include "odom.hpp"
#include "../config/config.hpp"
#include "../utils/utils.hpp"
#include "okapi/api/units/QLength.hpp"

// constants
const double BACK_ENC_OFFSET = 3.0;
const double TICKS_PER_CM = 13.0;

// the global position of the robot in an unspecified unit
odom::Point odom::globalPosition = {0, 0, 0};

// the processed position of the robot in cm
odom::ProcessedPoint odom::position = {0_cm, 0_cm, 0};

// other variables to help calculate the global position
odom::Point odom::previousPosition = {0, 0, 0};
odom::Point odom::globalDeltaPoint = {0, 0, 0};
odom::Point odom::localDeltaPoint = {0, 0, 0};

// structure to keep sensor values
struct EncoderData {
  double left, right, back;
};

// keep track of the last sensor values
EncoderData currentEncoderData = {0, 0, 0};
EncoderData lastEncoderData = {0, 0, 0};
EncoderData deltaEncoderData = {0, 0, 0};

// keep track of inertial sensor values
struct {
  double current, previous, delta;
} inertialSensorData = {0, 0, 0};

// Odometry Functions
void odom::updateSensors() {
  // get the left, right, and back encoder data
  currentEncoderData.left = odom_left->get();
  currentEncoderData.right = -odom_right->get();
  currentEncoderData.back = -odom_middle->get();

  // printf("left: %f, right: %f, back: %f\n", currentEncoderData.left,
  //        currentEncoderData.right, currentEncoderData.back);

  // find the delta encoder values
  deltaEncoderData.left = currentEncoderData.left - lastEncoderData.left;
  deltaEncoderData.right = currentEncoderData.right - lastEncoderData.right;
  deltaEncoderData.back = currentEncoderData.back - lastEncoderData.back;

  // printf("left: %f, right: %f, back: %f\n", deltaEncoderData.left,
  //        deltaEncoderData.right, deltaEncoderData.back);

  // update the last encoder values
  lastEncoderData.left = currentEncoderData.left;
  lastEncoderData.right = currentEncoderData.right;
  lastEncoderData.back = currentEncoderData.back;

  // get the inertial sensor value
  inertialSensorData.current = utils::getRadians(inertial->get_rotation());

  // find the delta inertial sensor value
  inertialSensorData.delta =
      inertialSensorData.current - inertialSensorData.previous;

  // update the previous inertial sensor value
  inertialSensorData.previous = inertialSensorData.current;
}

// update the global position
void odom::updatePosition() {
  // polar coordinates
  localDeltaPoint.x =
      (inertialSensorData.delta + (deltaEncoderData.back / BACK_ENC_OFFSET)) *
      BACK_ENC_OFFSET;

  localDeltaPoint.y = (deltaEncoderData.left + deltaEncoderData.right) / 2.0;

  // to cartesian coordinates
  globalDeltaPoint.x =
      (localDeltaPoint.y *
       sin(previousPosition.angle + globalDeltaPoint.angle / 2)) +
      (localDeltaPoint.x *
       cos(previousPosition.angle + globalDeltaPoint.angle / 2));

  globalDeltaPoint.y =
      (localDeltaPoint.y *
       cos(previousPosition.angle + globalDeltaPoint.angle / 2)) -
      (localDeltaPoint.x *
       sin(previousPosition.angle + globalDeltaPoint.angle / 2));

  // update the global position
  globalPosition.x = globalDeltaPoint.x + previousPosition.x;
  globalPosition.y = globalDeltaPoint.y + previousPosition.y;
  globalPosition.angle = inertialSensorData.current;

  // update the previous position
  previousPosition = globalPosition;
}

// reset the global position
void odom::resetPosition() {
  // reset the global position
  globalPosition.x = 0;
  globalPosition.y = 0;
  globalPosition.angle = 0;

  // reset the previous position
  previousPosition = globalPosition;

  // reset the inertial sensor
  inertial->reset();

  // reset the encoders
  drive_top_left->tarePosition();
  drive_top_right->tarePosition();
  drive_bottom_left->tarePosition();
  drive_bottom_right->tarePosition();
  odom_middle->reset();
}

// update the processed global position
void odom::updateProcessedPosition() {
  // update the processed global position
  // convert the global position to cm and make it a QLength
  position.x = globalPosition.x / TICKS_PER_CM * 1_cm;
  position.y = globalPosition.y / TICKS_PER_CM * 1_cm;
  position.angle = globalPosition.angle;
}

// odometry task
void odom::run() {
  while (true) {
    // wait for inertial sensor to calibrate
    while (inertial->is_calibrating())
      pros::delay(20);

    // update the sensors
    updateSensors();

    // update the global position
    updatePosition();

    // update the processed global position
    updateProcessedPosition();

    // print the global position
    printf("x: %f, y: %f, angle: %f\n", position.x.convert(centimeter),
           position.y.convert(centimeter), position.angle);

    // wait for the next iteration
    pros::delay(10);
  }
}