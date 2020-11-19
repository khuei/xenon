#include "port.h"

pros::Imu imu(5);

okapi::Controller controller;

okapi::Motor front_left(1, false, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
okapi::Motor back_left(11, false, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
okapi::Motor front_right(10, true, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
okapi::Motor back_right(20, true, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);

std::shared_ptr<okapi::ChassisController> chassis =
	okapi::ChassisControllerBuilder()
		.withMotors(front_left, front_right, back_right, back_left)
		.withDimensions(okapi::AbstractMotor::gearset::green, { { 4_in, 11.5_in }, okapi::imev5GreenTPR })
		.withMaxVelocity(100)
		.withOdometry()
		.buildOdometry();

std::shared_ptr<okapi::XDriveModel> drive = std::dynamic_pointer_cast<okapi::XDriveModel>(chassis->getModel());
