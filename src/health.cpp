#include "health.h"

namespace checkhealth {

std::string
battery(void)
{
	if (pros::battery::get_temperature() <= 55 || pros::battery::get_temperature() >= -10)
		return "OK";
	else
		return "FAILED";
}

std::string
controller(pros::Controller controller)
{
	if (controller.get_battery_level() >= 20)
		return "OK";
	else
		return "FAILED";
}

std::string
motor(std::shared_ptr<okapi::Motor> motor)
{
	if (!motor->isOverCurrent() && !motor->isOverTemp())
		return "OK";
	else
		return "FAILED";
}

std::string
usd(void)
{
	if (pros::usd::is_installed())
		return "OK";
	else
		return "FAILED";
}

} // namespace checkhealth
