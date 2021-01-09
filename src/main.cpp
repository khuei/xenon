#include <iostream>

#include "pros/rtos.hpp"
#include "pros/misc.hpp"

#include "main.h"
#include "port.h"
#include "config.h"

#include "debug.h"
#include "logger.h"
#include "gui.h"

#include "intake.h"
#include "chassis.h"

#include "purepursuit.h"

void
initialize(void)
{
	logger::init();
	debug::init();

	chassis::init();
	intake::init();
}

void
opcontrol(void)
{
	std::uint32_t now = pros::millis();
	for (;;) {
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A) && !pros::competition::is_connected())
			autonomous();

		chassis::arcade();

		switch (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
		case 0:
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
			    master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_internal(-intake::max_speed);
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
				 !master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_internal(intake::max_speed);
			else
				intake::run_internal(0);

			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
			    master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_front(-intake::max_speed);
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
				 !master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_front(intake::max_speed);
			else
				intake::run_front(0);
			break;
		case 1:
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
				intake::run_both(-intake::max_speed);
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
				intake::run_both(intake::max_speed);
			else
				intake::run_both(0);
			break;
		}

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X))
			intake::tuner(pros::E_CONTROLLER_DIGITAL_UP, pros::E_CONTROLLER_DIGITAL_DOWN);

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
			purepursuit::tuner(pros::E_CONTROLLER_DIGITAL_LEFT,
					   pros::E_CONTROLLER_DIGITAL_RIGHT,
					   pros::E_CONTROLLER_DIGITAL_UP,
					   pros::E_CONTROLLER_DIGITAL_DOWN);

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			switch (debug::started) {
			case false:
				debug::init();
				break;
			case true:
				debug::stop();
				break;
			}
		}

		if (debug::started) {
			debug::switch_tab(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);
			gui::switch_theme(pros::E_CONTROLLER_DIGITAL_DOWN, pros::E_CONTROLLER_DIGITAL_UP);
		}

		pros::Task::delay_until(&now, 10);
	}
}
