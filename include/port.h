#ifndef _PORT_H_
#define _PORT_H_

#include "okapi/api.hpp"
#include "pros/rtos.hpp"

using namespace okapi::literals;

extern pros::Controller master;

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

extern std::shared_ptr<okapi::OdomChassisController> drive;
extern std::shared_ptr<okapi::AsyncMotionProfileController> drive_profile;
extern std::shared_ptr<okapi::XDriveModel> x_drive;

#endif
