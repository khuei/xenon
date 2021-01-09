#ifndef CONFIG
#define CONFIG

#include "port.h"

namespace auton {

extern std::vector<okapi::Point> path;

} // namespace auton

namespace chassis {

extern double max_speed;
extern okapi::QLength distance_constant;
extern okapi::QLength turn_constant;

} // namespace chassis

namespace intake {

extern double max_speed;
extern double accel_step;

} // namespace intake

namespace purepursuit {

extern double max_speed;
extern double accel_step;

extern okapi::QLength inner_radius;
extern okapi::QLength outer_radius;

extern double kP_vel;
extern double kD_vel;

extern double kP_ang;
extern double kD_ang;

} // namespace purepursuit

#endif
