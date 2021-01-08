#ifndef _CHECKHEALTH_H_
#define _CHECKHEALTH_H_

#include "okapi/api.hpp"

namespace checkhealth {

std::string battery(void);
std::string controller(pros::Controller);
std::string motor(std::shared_ptr<okapi::Motor>);
std::string usd(void);

} // namespace checkhealth

#endif
