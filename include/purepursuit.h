#ifndef _PUREPURSUIT_H_
#define _PUREPURSUIT_H_

#include "port.h"

namespace purepursuit {

void tuner(pros::controller_digital_e_t,
	   pros::controller_digital_e_t,
	   pros::controller_digital_e_t,
	   pros::controller_digital_e_t);
void follow_path(std::vector<okapi::Point>);

} // namespace purepursuit

#endif
