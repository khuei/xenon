#ifndef _PUREPURSUIT_H_
#define _PUREPURSUIT_H_

#include "port.h"

namespace purepursuit {

extern double max_speed;
extern double accel_step;

extern okapi::QLength inner_radius;
extern okapi::QLength outer_radius;

extern double kP_vel;
extern double kD_vel;

extern double kP_ang;
extern double kD_ang;

extern double left_speed;
extern double right_speed;

void tuner(pros::controller_digital_e_t,
	   pros::controller_digital_e_t,
	   pros::controller_digital_e_t,
	   pros::controller_digital_e_t);
void follow_path(std::vector<okapi::Point>);

} // namespace purepursuit

#endif
