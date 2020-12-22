#ifndef PORT
#define PORT

#include "okapi/api.hpp"

using namespace okapi::literals;

extern okapi::Controller controller;

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
