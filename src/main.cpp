#include <iostream>

#include "main.h"
#include "port.h"
#include "util.h"
#include "intake.h"
#include "logger.h"
#include "chassis.h"
#include "gui.h"

void
initialize(void)
{
	gui::init();

	fclose(fopen("/usd/event.log", "w"));

	logger::elog("pros: start initialization");

	okapi::Logger::setDefaultLogger(okapi_logger);

	if (imu.calibrate())
		logger::elog("imu: calibrated");
	else
		logger::elog("imu: failed to calibrated");

	while (imu.isCalibrating())
		rate.delayUntil(10_ms);

	logger::elog("chassis: reset");
	chassis::reset();

	logger::elog("chassis: set brake mode to brake");
	chassis::set_brake(okapi::AbstractMotor::brakeMode::brake);

	logger::elog("intake: reset");
	intake::reset();

	logger::elog("intake: set brake mode to coast");
	intake::set_brake(okapi::AbstractMotor::brakeMode::coast);

	logger::elog("pros: end initialization\n");
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

		rate.delayUntil(10_ms);
	}
}
