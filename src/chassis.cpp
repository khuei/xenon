#include "pros/rtos.hpp"

#include "chassis.h"
#include "logger.h"

namespace chassis {

void
reset(void)
{
	front_left->tarePosition();
	back_left->tarePosition();
	front_right->tarePosition();
	back_right->tarePosition();
}

void
set_brake(okapi::AbstractMotor::brakeMode brake_mode)
{
	front_left->setBrakeMode(brake_mode);
	back_left->setBrakeMode(brake_mode);
	front_right->setBrakeMode(brake_mode);
	back_right->setBrakeMode(brake_mode);
}

void
arcade(void)
{
	x_drive->xArcade(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X),
			 master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
			 master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
}

void
init(void)
{
	imu->calibrate();
	while (imu->isCalibrating())
		pros::delay(10);
	logger::elog("imu: calibrated");

	chassis::reset();
	logger::elog("chassis: reset");

	chassis::set_brake(okapi::AbstractMotor::brakeMode::brake);
	logger::elog("chassis: set brake mode to brake");
}

} // namespace chassis
