#ifndef ODOM
#define ODOM

#include "port.h"

namespace odom {

okapi::QAngle get_delta_theta(okapi::Point);
okapi::QLength get_delta_dist(okapi::Point);

} // namespace odom

#endif
