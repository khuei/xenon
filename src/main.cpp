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
	logger::init();

	chassis::init();
	intake::init();
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

		gui::reload_diagnostic();

		rate.delayUntil(10_ms);
	}
}
