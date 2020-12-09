#include "intake.h"

namespace intake {

void
reset(void)
{
	left_intake.tarePosition();
	right_intake.tarePosition();
	top_intake.tarePosition();
	bottom_intake.tarePosition();
}

void
set_brake(okapi::AbstractMotor::brakeMode brake_mode)
{
	left_intake.setBrakeMode(brake_mode);
	right_intake.setBrakeMode(brake_mode);
	top_intake.setBrakeMode(brake_mode);
	bottom_intake.setBrakeMode(brake_mode);
}

void
run_front(double speed)
{
	left_intake.moveVelocity(speed);
	right_intake.moveVelocity(speed);
}

void
run_internal(double speed)
{
	top_intake.moveVelocity(speed);
	bottom_intake.moveVelocity(speed);
}

void
run_both(double speed)
{
	left_intake.moveVelocity(speed);
	right_intake.moveVelocity(speed);
	top_intake.moveVelocity(speed);
	bottom_intake.moveVelocity(speed);
}

} // namespace intake
