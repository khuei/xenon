#include "port.h"

okapi::Controller controller;

std::shared_ptr<okapi::IMU> imu = std::make_shared<okapi::IMU>(5, okapi::IMUAxes::z);

std::shared_ptr<okapi::Logger> okapi_logger =
	std::make_shared<okapi::Logger>(okapi::TimeUtilFactory::createDefault().getTimer(),
					"/usd/event.log",
					okapi::Logger::LogLevel::debug);

std::shared_ptr<okapi::Motor> front_left = std::make_shared<okapi::Motor>(1,
									  false,
									  okapi::AbstractMotor::gearset::green,
									  okapi::AbstractMotor::encoderUnits::degrees,
									  okapi::Logger::getDefaultLogger());
std::shared_ptr<okapi::Motor> back_left = std::make_shared<okapi::Motor>(11,
									 false,
									 okapi::AbstractMotor::gearset::green,
									 okapi::AbstractMotor::encoderUnits::degrees,
									 okapi::Logger::getDefaultLogger());
std::shared_ptr<okapi::Motor> front_right = std::make_shared<okapi::Motor>(10,
									   true,
									   okapi::AbstractMotor::gearset::green,
									   okapi::AbstractMotor::encoderUnits::degrees,
									   okapi::Logger::getDefaultLogger());
std::shared_ptr<okapi::Motor> back_right = std::make_shared<okapi::Motor>(20,
									  true,
									  okapi::AbstractMotor::gearset::green,
									  okapi::AbstractMotor::encoderUnits::degrees,
									  okapi::Logger::getDefaultLogger());

std::shared_ptr<okapi::Motor> top_intake = std::make_shared<okapi::Motor>(8,
									  true,
									  okapi::AbstractMotor::gearset::green,
									  okapi::AbstractMotor::encoderUnits::degrees,
									  okapi::Logger::getDefaultLogger());
std::shared_ptr<okapi::Motor> bottom_intake =
	std::make_shared<okapi::Motor>(9,
				       false,
				       okapi::AbstractMotor::gearset::green,
				       okapi::AbstractMotor::encoderUnits::degrees,
				       okapi::Logger::getDefaultLogger());

std::shared_ptr<okapi::Motor> left_intake = std::make_shared<okapi::Motor>(12,
									   false,
									   okapi::AbstractMotor::gearset::green,
									   okapi::AbstractMotor::encoderUnits::degrees,
									   okapi::Logger::getDefaultLogger());
std::shared_ptr<okapi::Motor> right_intake = std::make_shared<okapi::Motor>(19,
									    true,
									    okapi::AbstractMotor::gearset::green,
									    okapi::AbstractMotor::encoderUnits::degrees,
									    okapi::Logger::getDefaultLogger());

std::shared_ptr<okapi::OdomChassisController> drive =
	okapi::ChassisControllerBuilder()
		.withMotors(front_left, front_right, back_right, back_left)
		.withDimensions(okapi::AbstractMotor::gearset::green, { { 3.85_in, 22.75_in }, okapi::imev5GreenTPR })
		.withMaxVelocity(100)
		.withLogger(okapi::Logger::getDefaultLogger())
		.withOdometry()
		.buildOdometry();

std::shared_ptr<okapi::AsyncMotionProfileController> drive_profile =
	okapi::AsyncMotionProfileControllerBuilder()
		.withLimits({1, 2, 10})
		.withOutput(drive)
		.withLogger(okapi::Logger::getDefaultLogger())
		.buildMotionProfileController();

std::shared_ptr<okapi::XDriveModel> x_drive = std::dynamic_pointer_cast<okapi::XDriveModel>(drive->getModel());
