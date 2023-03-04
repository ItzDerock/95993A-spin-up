#include "main.h"
#include <iomanip>

#define _USE_MATH_DEFINES
#include <math.h>

#include "../odom/odom.hpp"

namespace utils {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// takes a value and compresses it to fit in a range.
double mapValue(double value, double istart, double istop, double ostart,
                double ostop);

// round to x decimal (string)
std::string round(double value, int decimal);

// conversions (distance)
double degToInch(double deg);
double inchToDeg(double inch);

// conversions (angles)
double getRadians(double deg);
double getDegrees(double rad);
double angleWrap(double angle);
double compressAngle(double startAngle, double angle);

// constrain
double constrain(double value, double min, double max);

// degrees to face target
double getAngleToTarget(double x1, double y1, double x2, double y2);

// distance
double getDistance(double x1, double y1, double x2, double y2);
double getDistance(odom::XY point1, odom::XY point2);
} // namespace utils
