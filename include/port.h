#ifndef _PORT_H_
#define _PORT_H_

#include "okapi/api.hpp"
#include "pros/optical.hpp"
#include "pros/vision.hpp"
#include "pros/rtos.hpp"

using namespace okapi::literals;

extern pros::Controller master;

extern std::shared_ptr<pros::Optical> optical;
extern std::shared_ptr<pros::Vision> vision;

extern std::shared_ptr<okapi::IMU> imu;

extern std::shared_ptr<okapi::Logger> okapi_logger;

extern std::shared_ptr<okapi::Motor> front_left;
extern std::shared_ptr<okapi::Motor> back_left;
extern std::shared_ptr<okapi::Motor> front_right;
extern std::shared_ptr<okapi::Motor> back_right;

extern std::shared_ptr<okapi::Motor> top_intake;
extern std::shared_ptr<okapi::Motor> bottom_intake;

extern std::shared_ptr<okapi::Motor> left_intake;
extern std::shared_ptr<okapi::Motor> right_intake;

extern std::shared_ptr<okapi::ChassisController> internal_intake;
extern std::shared_ptr<okapi::ChassisController> front_intake;
extern std::shared_ptr<okapi::SkidSteerModel> internal_model;
extern std::shared_ptr<okapi::SkidSteerModel> front_model;

extern std::shared_ptr<okapi::ChassisController> left_diagonal;
extern std::shared_ptr<okapi::ChassisController> right_diagonal;
extern std::shared_ptr<okapi::OdomChassisController> drive;
extern std::shared_ptr<okapi::AsyncMotionProfileController> drive_profile;
extern std::shared_ptr<okapi::XDriveModel> x_drive;

#endif
