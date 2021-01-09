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
	gui::init();

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
			config::switch_tuner(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
			switch (config::current_tuner) {
			case 0:
				chassis::tuner(pros::E_CONTROLLER_DIGITAL_LEFT,
						   pros::E_CONTROLLER_DIGITAL_RIGHT,
						   pros::E_CONTROLLER_DIGITAL_UP,
						   pros::E_CONTROLLER_DIGITAL_DOWN);
				break;
			case 1:
				intake::tuner(pros::E_CONTROLLER_DIGITAL_LEFT,
						   pros::E_CONTROLLER_DIGITAL_RIGHT,
						   pros::E_CONTROLLER_DIGITAL_UP,
						   pros::E_CONTROLLER_DIGITAL_DOWN);
				break;
			case 2:
				purepursuit::tuner(pros::E_CONTROLLER_DIGITAL_LEFT,
						   pros::E_CONTROLLER_DIGITAL_RIGHT,
						   pros::E_CONTROLLER_DIGITAL_UP,
						   pros::E_CONTROLLER_DIGITAL_DOWN);
				break;
			}
		}

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
			gui::switch_display(pros::E_CONTROLLER_DIGITAL_UP);

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
			gui::toggle(pros::E_CONTROLLER_DIGITAL_DOWN);

		if (gui::started) {
			gui::switch_tab(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);
			gui::switch_theme(pros::E_CONTROLLER_DIGITAL_DOWN, pros::E_CONTROLLER_DIGITAL_UP);
		}

		pros::Task::delay_until(&now, 10);
	}
}
