#include <iostream>

#include "okapi/api.hpp"

#include "util.h"

namespace util {

void
switcher(int *num, int total)
{
	if (*num < total)
		++(*num);
	else
		*num = 0;
}

double
slew(double *current_speed, double step, double target_speed)
{
	if (target_speed > *current_speed + step)
		*current_speed += step;
	else if (target_speed < *current_speed - step)
		*current_speed -= step;
	else
		*current_speed = target_speed;

	return *current_speed;
}

} // namespace util
