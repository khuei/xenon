#include <iostream>

#include "odom.h"

namespace odom {

okapi::QAngle
get_delta_theta(std::array<okapi::QLength, 2> coord)
{
	okapi::QLength global_x = drive->getState().x;
	okapi::QLength global_y = drive->getState().y;
	okapi::QAngle global_theta = drive->getState().theta;

	okapi::QLength x = coord[0];
	okapi::QLength y = coord[1];
	okapi::QAngle delta_theta;

	x -= global_x;
	y -= global_y;

	delta_theta = global_theta - atan2(x, y);

	while (delta_theta < (okapi::QAngle)-okapi::pi)
		delta_theta += 2 * (okapi::QAngle)okapi::pi;

	while (delta_theta > (okapi::QAngle)okapi::pi)
		delta_theta -= 2 * (okapi::QAngle)okapi::pi;

	return delta_theta;
}

okapi::QLength
get_delta_dist(std::array<okapi::QLength, 2> coord)
{
	okapi::QLength global_x = drive->getState().x;
	okapi::QLength global_y = drive->getState().y;

	okapi::QLength x = coord[0];
	okapi::QLength y = coord[1];

	x -= global_x;
	y -= global_y;

	return sqrt(x * x + y * y);
}

}
