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
	if (!controller.getDigital(okapi::ControllerDigital::L2)) {
		x_drive->xArcade(
			controller.getAnalog(okapi::ControllerAnalog::leftX),
			controller.getAnalog(okapi::ControllerAnalog::leftY),
			controller.getAnalog(okapi::ControllerAnalog::rightX));
	} else if (controller.getDigital(okapi::ControllerDigital::L2)) {
		/* move left diagonally */
		front_right.moveVelocity(
			-100 *
			controller.getAnalog(okapi::ControllerAnalog::leftY));
		back_left.moveVelocity(
			-100 *
			controller.getAnalog(okapi::ControllerAnalog::leftY));

		/* move right diagonally */
		front_left.moveVelocity(
			100 *
			controller.getAnalog(okapi::ControllerAnalog::rightY));
		back_right.moveVelocity(
			100 *
			controller.getAnalog(okapi::ControllerAnalog::rightY));
	}
}

} // namespace chassis
