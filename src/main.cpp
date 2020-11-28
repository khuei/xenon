#include <iostream>

#include "main.h"
#include "port.h"
#include "util.h"

#include "chassis.h"

void
initialize(void)
{
	if (imu.reset())
		fprintf(stdout, "imu: calibrated");
	else
		fprintf(stderr, "imu: failed to calibrated");

	imu.reset();
	chassis::reset();

	chassis::set_brake(okapi::AbstractMotor::brakeMode::brake);
}

void
opcontrol(void)
{
	for (;;) {
		chassis::arcade();
	}
}
