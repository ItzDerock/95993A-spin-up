#include "./utils.hpp"

// takes a value and compresses it to fit in a range.
double utils::mapValue(double value, double istart, double istop, double ostart,
                       double ostop) {
  return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

// round to x decimal (string)
std::string utils::round(double value, int decimal) {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(decimal) << value;
  return stream.str();
}

// conversions (distance)
double utils::degToInch(double deg) { return (deg / 360) * (M_PI * 2.75); }
double utils::inchToDeg(double inch) { return (inch / (M_PI * 2.75)) * 360; }

// conversions (angles)
double utils::getRadians(double deg) { return (deg * M_PI) / 180; }
double utils::getDegrees(double rad) { return (rad * 180) / M_PI; }
double utils::angleWrap(double angle) {
  while (angle < -M_PI) {
    angle += 2 * M_PI;
  }
  while (angle > M_PI) {
    angle -= 2 * M_PI;
  }
  return angle;
}

double utils::compressAngle(double startAngle, double angle) {
  while (angle <= startAngle - M_PI * 2) {
    angle += M_PI * 2;
  }
  while (angle >= startAngle + M_PI * 2) {
    angle -= M_PI * 2;
  }
  return angle;
}

// constrain
double utils::constrain(double value, double min, double max) {
  if (value < min) {
    return min;
  } else if (value > max) {
    return max;
  } else {
    return value;
  }
}