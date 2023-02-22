#include "main.h"
#include <iomanip>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace utils {

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

} // namespace utils