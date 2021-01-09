#include "config.h"
#include "logger.h"

namespace chassis {

double max_speed = 100;
okapi::QLength distance_constant = 3.85_in;
okapi::QLength turn_constant = 22.75_in;

} // namespace chassis

namespace intake {

double max_speed = 200;
double accel_step = 5;

} // namespace intake

namespace purepursuit {

double max_speed = 80;
double accel_step = 5;

okapi::QLength inner_radius = 20.0_in;
okapi::QLength outer_radius = 25.0_in;

double kP_vel = 0.08;
double kD_vel = 0.08;

double kP_ang = 0.08;
double kD_ang = 0.08;

double left_speed = 0;
double right_speed = 0;

} // namespace purepursuit
