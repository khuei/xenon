#ifndef ODOM
#define ODOM

#include "port.h"

namespace odom {

okapi::QAngle get_delta_theta(std::array<okapi::QLength, 2>);
okapi::QLength get_delta_dist(std::array<okapi::QLength, 2>);

} // namespace odom

#endif
