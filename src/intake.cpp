#include "intake.h"
#include "util.h"
#include "logger.h"

namespace intake {

double accel_step = 5;

double top_speed = 0;
double bottom_speed = 0;

double left_speed = 0;
double right_speed = 0;

void
tuner(pros::controller_digital_e_t increase, pros::controller_digital_e_t decrease)
{
	master.print(2, 0, "accel_step: %f", accel_step);
	if (master.get_digital_new_press(increase))
		accel_step += 1;
	if (master.get_digital_new_press(decrease))
		accel_step -= 1;
}

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

	intake::set_brake(okapi::AbstractMotor::brakeMode::coast);
	logger::elog("intake: set brake mode to coast");
}

} // namespace intake
