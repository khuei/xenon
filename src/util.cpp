#include <iostream>
#include <cstdlib>
#include <cstdarg>

namespace util {

void
die(const char *errstr, ...)
{
	va_list ap;

	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);

	exit(1);
}

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
