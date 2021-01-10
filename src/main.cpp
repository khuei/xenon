#include <iostream>

#include "pros/rtos.hpp"
#include "pros/misc.hpp"

#include "main.h"
#include "port.h"
#include "config.h"

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
		intake::opcontrol();

		config::tuner();


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
