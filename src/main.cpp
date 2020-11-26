#include <iostream>

#include "main.h"
#include "port.h"
#include "util.h"

void
initialize(void)
{
	imu.reset();

	front_left.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
	back_left.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
	front_right.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
	back_right.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
}

void
opcontrol(void)
{
	for (;;) {
		if (!controller.getDigital(okapi::ControllerDigital::L2)) {
			drive->xArcade(
				controller.getAnalog(
					okapi::ControllerAnalog::leftX),
				controller.getAnalog(
					okapi::ControllerAnalog::leftY),
				controller.getAnalog(
					okapi::ControllerAnalog::rightX));
		} else if (controller.getDigital(
				   okapi::ControllerDigital::L2)) {
			/* move left diagonally */
			front_right.moveVelocity(
				-100 * controller.getAnalog(
					       okapi::ControllerAnalog::leftY));
			back_left.moveVelocity(
				-100 * controller.getAnalog(
					       okapi::ControllerAnalog::leftY));

			/* move right diagonally */
			front_left.moveVelocity(
				100 * controller.getAnalog(
					      okapi::ControllerAnalog::rightY));
			back_right.moveVelocity(
				100 * controller.getAnalog(
					      okapi::ControllerAnalog::rightY));
		}
	}
}
