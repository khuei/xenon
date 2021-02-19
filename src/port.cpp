#include "port.h"
#include "config.h"
#include "logger.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

std::shared_ptr<okapi::IMU> imu = std::make_shared<okapi::IMU>(5, okapi::IMUAxes::z);

std::shared_ptr<okapi::Logger> okapi_logger =
	std::make_shared<okapi::Logger>(okapi::TimeUtilFactory::createDefault().getTimer(),
					logger::logfile(),
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
									  false,
									  okapi::AbstractMotor::gearset::green,
									  okapi::AbstractMotor::encoderUnits::degrees,
									  okapi::Logger::getDefaultLogger());
std::shared_ptr<okapi::Motor> bottom_intake =
	std::make_shared<okapi::Motor>(2,
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

std::shared_ptr<okapi::ChassisController> internal_intake =
	okapi::ChassisControllerBuilder()
		.withMotors(bottom_intake, top_intake)
		.withDimensions(okapi::AbstractMotor::gearset::green,
				{ { chassis::distance_constant, chassis::turn_constant }, okapi::imev5GreenTPR })
		.withClosedLoopControllerTimeUtil(std::numeric_limits<double>::max(), 10, 50_ms)
		.withMaxVelocity(intake::max_speed)
		.withLogger(okapi::Logger::getDefaultLogger())
		.build();

std::shared_ptr<okapi::ChassisController> front_intake =
	okapi::ChassisControllerBuilder()
		.withMotors(left_intake, right_intake)
		.withDimensions(okapi::AbstractMotor::gearset::green,
				{ { chassis::distance_constant, chassis::turn_constant }, okapi::imev5GreenTPR })
		.withClosedLoopControllerTimeUtil(std::numeric_limits<double>::max(), 10, 50_ms)
		.withMaxVelocity(intake::max_speed)
		.withLogger(okapi::Logger::getDefaultLogger())
		.build();

std::shared_ptr<okapi::SkidSteerModel> internal_model = std::dynamic_pointer_cast<okapi::SkidSteerModel>(internal_intake->getModel());
std::shared_ptr<okapi::SkidSteerModel> front_model = std::dynamic_pointer_cast<okapi::SkidSteerModel>(front_intake->getModel());

std::shared_ptr<okapi::ChassisController> left_diagonal =
	okapi::ChassisControllerBuilder()
		.withMotors(back_left, front_right)
		.withDimensions(okapi::AbstractMotor::gearset::green,
				{ { chassis::distance_constant, chassis::turn_constant }, okapi::imev5GreenTPR })
		.withClosedLoopControllerTimeUtil(std::numeric_limits<double>::max(), 10, 50_ms)
		.withMaxVelocity(chassis::max_speed)
		.withLogger(okapi::Logger::getDefaultLogger())
		.build();

std::shared_ptr<okapi::ChassisController> right_diagonal =
	okapi::ChassisControllerBuilder()
		.withMotors(front_left, back_right)
		.withDimensions(okapi::AbstractMotor::gearset::green,
				{ { chassis::distance_constant, chassis::turn_constant }, okapi::imev5GreenTPR })
		.withClosedLoopControllerTimeUtil(std::numeric_limits<double>::max(), 10, 50_ms)
		.withMaxVelocity(chassis::max_speed)
		.withLogger(okapi::Logger::getDefaultLogger())
		.withOdometry(okapi::StateMode::CARTESIAN)
		.build();

std::shared_ptr<okapi::OdomChassisController> drive =
	okapi::ChassisControllerBuilder()
		.withMotors(front_left, front_right, back_right, back_left)
		.withDimensions(okapi::AbstractMotor::gearset::green,
				{ { chassis::distance_constant, chassis::turn_constant }, okapi::imev5GreenTPR })
		.withClosedLoopControllerTimeUtil(std::numeric_limits<double>::max(), 10, 50_ms)
		.withMaxVelocity(chassis::max_speed)
		.withLogger(okapi::Logger::getDefaultLogger())
		.withOdometry(okapi::StateMode::CARTESIAN)
		.buildOdometry();

std::shared_ptr<okapi::AsyncMotionProfileController> drive_profile =
	okapi::AsyncMotionProfileControllerBuilder()
		.withLimits({ 1, 2, 10 })
		.withTimeUtilFactory(okapi::ConfigurableTimeUtilFactory(std::numeric_limits<double>::max(), 10, 50_ms))
		.withOutput(drive)
		.withLogger(okapi::Logger::getDefaultLogger())
		.buildMotionProfileController();

std::shared_ptr<okapi::XDriveModel> x_drive = std::dynamic_pointer_cast<okapi::XDriveModel>(drive->getModel());
