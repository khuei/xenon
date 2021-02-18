#include "intake.h"
#include "util.h"
#include "logger.h"
#include "config.h"

namespace intake {

double top_speed = 0;
double bottom_speed = 0;

double left_speed = 0;
double right_speed = 0;

void
reset(void)
{
	left_intake->tarePosition();
	right_intake->tarePosition();
	top_intake->tarePosition();
	bottom_intake->tarePosition();
}

void
set_brake(okapi::AbstractMotor::brakeMode brake_mode)
{
	left_intake->setBrakeMode(brake_mode);
	right_intake->setBrakeMode(brake_mode);
	top_intake->setBrakeMode(brake_mode);
	bottom_intake->setBrakeMode(brake_mode);
}

void
run_front(double max_speed)
{
	left_speed = util::slew(&left_speed, accel_step, max_speed);
	right_speed = util::slew(&right_speed, accel_step, max_speed);

	left_intake->moveVelocity(left_speed);
	right_intake->moveVelocity(right_speed);
}

void
run_internal(double max_speed)
{
	top_speed = util::slew(&top_speed, accel_step, max_speed);
	bottom_speed = util::slew(&bottom_speed, accel_step, max_speed);

	top_intake->moveVelocity(top_speed);
	bottom_intake->moveVelocity(bottom_speed);
}

void
run_both(double speed)
{
	run_front(speed);
	run_internal(speed);
}

void
init(void)
{
	intake::reset();
	logger::elog("intake: reset");

	intake::set_brake(okapi::AbstractMotor::brakeMode::hold);
	logger::elog("intake: set brake mode to hold");
}

void
opcontrol(void)
{
	if (!intake::baby_mode) {
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
	} else {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
			intake::run_front(intake::max_speed);
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
			intake::run_front(-intake::max_speed);
		else
			intake::run_front(0);

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
			intake::run_internal(intake::max_speed);
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
			intake::run_internal(-intake::max_speed);
		else
			intake::run_internal(0);
	}
}

} // namespace intake
