#include "pros/rtos.hpp"

#include "chassis.h"
#include "config.h"
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
move(okapi::QLength x, okapi::QLength y, okapi::QAngle theta)
{
	okapi::PathfinderPoint start = {0_ft, 0_ft, 0_deg};
	okapi::PathfinderPoint target = {y, x, theta};
	bool reversed = false;

	if (y < 0_in)
		reversed = true;

	drive_profile->moveTo({start, target}, reversed);
}

void
arcade(void)
{
	x_drive->xArcade((double)master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) / 127,
			 (double)master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127,
			 (double)master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) / 127);
}

void
init(void)
{
	if (!std::isnan(imu->get())) {
		imu->calibrate();
		while (imu->isCalibrating())
			pros::delay(10);
		logger::elog("imu: calibrated");
	}

	chassis::reset();
	logger::elog("chassis: reset");

	chassis::set_brake(okapi::AbstractMotor::brakeMode::hold);
	logger::elog("chassis: set brake mode to hold");

	drive->setMaxVelocity(chassis::max_speed);
}

} // namespace chassis
