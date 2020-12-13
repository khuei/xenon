#include "chassis.h"

namespace chassis {

void
reset(void)
{
	front_left.tarePosition();
	back_left.tarePosition();
	front_right.tarePosition();
	back_right.tarePosition();
}

void
set_brake(okapi::AbstractMotor::brakeMode brake_mode)
{
	front_left.setBrakeMode(brake_mode);
	back_left.setBrakeMode(brake_mode);
	front_right.setBrakeMode(brake_mode);
	back_right.setBrakeMode(brake_mode);
}

void
arcade(void)
{
	x_drive->xArcade(controller.getAnalog(okapi::ControllerAnalog::leftX),
			 controller.getAnalog(okapi::ControllerAnalog::leftY),
			 controller.getAnalog(okapi::ControllerAnalog::rightX));
}

} // namespace chassis
