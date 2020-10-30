#ifndef PORT
#define PORT

#include "okapi/api.hpp"
#include "pros/imu.hpp"

using namespace okapi::literals;

extern pros::Imu imu;

extern okapi::Controller controller;

extern okapi::Motor front_left;
extern okapi::Motor back_left;
extern okapi::Motor front_right;
extern okapi::Motor back_right;

extern std::shared_ptr<okapi::ChassisController> chassis;
extern std::shared_ptr<okapi::XDriveModel> drive;

#endif
