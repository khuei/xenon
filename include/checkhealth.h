#ifndef CHECKHEALTH
#define CHECKHEALTH

#include "okapi/api.hpp"

namespace checkhealth {

std::string battery(void);
std::string controller(pros::Controller);
std::string motor(std::shared_ptr<okapi::Motor>);
std::string usd(void);

} // namespace checkhealth

#endif
