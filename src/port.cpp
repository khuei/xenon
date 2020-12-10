#include "port.h"

pros::Imu imu(5);

okapi::Controller controller;

okapi::Motor front_left(1, false, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
okapi::Motor back_left(11, false, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
okapi::Motor front_right(10, true, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
okapi::Motor back_right(20, true, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);

okapi::Motor top_intake(8, true, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
okapi::Motor bottom_intake(9, false, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);

okapi::Motor left_intake(12, false, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
okapi::Motor right_intake(19, true, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);

std::shared_ptr<okapi::ChassisController> drive =
	okapi::ChassisControllerBuilder()
		.withMotors(front_left, front_right, back_right, back_left)
		.withDimensions(okapi::AbstractMotor::gearset::green, { { 3.85_in, 22.75_in }, okapi::imev5GreenTPR })
		.withMaxVelocity(100)
		.withOdometry()
		.buildOdometry();

std::shared_ptr<okapi::XDriveModel> x_drive = std::dynamic_pointer_cast<okapi::XDriveModel>(drive->getModel());
