#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "port.h"

namespace auton {

extern std::vector<okapi::Point> path;
extern std::string alliance_color;

} // namespace auton

namespace chassis {

extern double max_speed;
extern okapi::QLength distance_constant;
extern okapi::QLength turn_constant;

} // namespace chassis

namespace intake {

extern bool baby_mode;
extern bool auto_sort;
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

namespace config {

extern bool started;

void switch_tab(pros::controller_digital_e_t, pros::controller_digital_e_t);

void tuner(void);
void init(void);
void stop(void);

} // namespace config
#endif
