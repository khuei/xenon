#include <iostream>

#include "main.h"
#include "port.h"
#include "util.h"
#include "intake.h"

#include "chassis.h"

void
initialize(void)
{
	okapi::Logger::setDefaultLogger(okapi_logger);

	if (imu.calibrate())
		fprintf(stdout, "imu: calibrated");
	else
		fprintf(stderr, "imu: failed to calibrated");

	while (imu.isCalibrating())
		rate.delayUntil(10_ms);

	chassis::reset();
	chassis::set_brake(okapi::AbstractMotor::brakeMode::brake);

	intake::reset();
	intake::set_brake(okapi::AbstractMotor::brakeMode::coast);
}

void
opcontrol(void)
{
	for (;;) {
		chassis::arcade();

		switch (controller.getDigital(okapi::ControllerDigital::L1)) {
		case 0:
			if (controller.getDigital(okapi::ControllerDigital::R1) &&
			    controller.getDigital(okapi::ControllerDigital::L2))
				intake::run_internal(-200);
			else if (controller.getDigital(okapi::ControllerDigital::R1) &&
				 !controller.getDigital(okapi::ControllerDigital::L2))
				intake::run_internal(200);
			else
				intake::run_internal(0);

			if (controller.getDigital(okapi::ControllerDigital::R2) &&
			    controller.getDigital(okapi::ControllerDigital::L2))
				intake::run_front(-200);
			else if (controller.getDigital(okapi::ControllerDigital::R2) &&
				 !controller.getDigital(okapi::ControllerDigital::L2))
				intake::run_front(200);
			else
				intake::run_front(0);
			break;
		case 1:
			if (controller.getDigital(okapi::ControllerDigital::R1))
				intake::run_both(-200);
			else if (controller.getDigital(okapi::ControllerDigital::R2))
				intake::run_both(200);
			else
				intake::run_both(0);
			break;
		}
	}
}
