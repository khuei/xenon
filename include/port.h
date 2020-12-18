#ifndef PORT
#define PORT

#include "okapi/api.hpp"

using namespace okapi::literals;

extern okapi::Rate rate;

extern okapi::IMU imu;

extern okapi::Controller controller;

extern std::shared_ptr<okapi::Logger> okapi_logger;

extern okapi::Motor front_left;
extern okapi::Motor back_left;
extern okapi::Motor front_right;
extern okapi::Motor back_right;

extern okapi::Motor top_intake;
extern okapi::Motor bottom_intake;

extern okapi::Motor left_intake;
extern okapi::Motor right_intake;

extern std::shared_ptr<okapi::ChassisController> drive;
extern std::shared_ptr<okapi::XDriveModel> x_drive;

#endif
