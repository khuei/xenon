#include <iostream>

#include "pros/rtos.hpp"
#include "pros/misc.hpp"

#include "main.h"
#include "port.h"

#include "gui.h"
#include "logger.h"

#include "intake.h"
#include "chassis.h"

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
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A) && !pros::competition::is_connected())
			autonomous();

		chassis::arcade();

		switch (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
		case 0:
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
			    master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_internal(-200);
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
				 !master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_internal(200);
			else
				intake::run_internal(0);

			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
			    master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_front(-200);
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
				 !master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_front(200);
			else
				intake::run_front(0);
			break;
		case 1:
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
				intake::run_both(-200);
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
				intake::run_both(200);
			else
				intake::run_both(0);
			break;
		}

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			switch (gui::started) {
			case false:
				gui::init();
				break;
			case true:
				gui::stop();
				break;
			}
		}

		if (gui::started)
			gui::switch_tab(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);

		pros::delay(10);
	}
}
