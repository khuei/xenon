#include "purepursuit.h"
#include "odom.h"
#include "util.h"

#include "pros/rtos.hpp"

namespace purepursuit {

enum tune_value {
	MAX_SPEED,
	ACCEL_STEP,
	INNER_RADIUS,
	OUTER_RADIUS,
	KP_VEL,
	KD_VEL,
	KP_ANG,
	KD_ANG,
};

int current_tune_value = 0;

double max_speed = 80;
double accel_step = 5;

okapi::QLength inner_radius = 20.0_in;
okapi::QLength outer_radius = 25.0_in;

double kP_vel = 0.08;
double kD_vel = 0.08;

double kP_ang = 0.08;
double kD_ang = 0.08;

double left_speed = 0;
double right_speed = 0;

bool last_segment = false;

void
tuner(pros::controller_digital_e_t left,
      pros::controller_digital_e_t right,
      pros::controller_digital_e_t increase,
      pros::controller_digital_e_t decrease)
{
	if (master.get_digital_new_press(left)) {
		master.clear_line(2);
		if (current_tune_value != MAX_SPEED)
			current_tune_value--;
		else
			current_tune_value = KD_ANG;
	}

	if (master.get_digital_new_press(right)) {
		master.clear_line(2);
		if (current_tune_value != KD_ANG)
			current_tune_value++;
		else
			current_tune_value = MAX_SPEED;
	}

	switch (current_tune_value) {
	case MAX_SPEED:
		master.print(2, 0, "max_speed: %f", purepursuit::max_speed);
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
			purepursuit::max_speed += 2;
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
			purepursuit::max_speed -= 2;
		break;
	case ACCEL_STEP:
		master.print(2, 0, "accel_step: %f", purepursuit::accel_step);
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
			purepursuit::accel_step += 1;
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
			purepursuit::accel_step -= 1;
		break;
	case INNER_RADIUS:
		master.print(2, 0, "inner_radius: %s", std::to_string(purepursuit::inner_radius.convert(okapi::inch)));
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
			purepursuit::inner_radius += 1_in;
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
			purepursuit::inner_radius -= 1_in;
		break;
	case OUTER_RADIUS:
		master.print(2, 0, "outer_radius: %s", std::to_string(purepursuit::outer_radius.convert(okapi::inch)));
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
			purepursuit::outer_radius += 1_in;
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
			purepursuit::outer_radius -= 1_in;
		break;
	case KP_VEL:
		master.print(2, 0, "kP_vel: %f", purepursuit::kP_vel);
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
			purepursuit::kP_vel += 0.001;
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
			purepursuit::kP_vel -= 0.001;
		break;
	case KD_VEL:
		master.print(2, 0, "kD_vel: %f", purepursuit::kD_vel);
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
			purepursuit::kD_vel += 0.001;
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
			purepursuit::kD_vel -= 0.001;
		break;
	case KP_ANG:
		master.print(2, 0, "kP_ang: %f", purepursuit::kP_ang);
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
			purepursuit::kP_ang += 0.001;
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
			purepursuit::kP_ang -= 0.001;
		break;
	case KD_ANG:
		master.print(2, 0, "kD_ang: %f", purepursuit::kD_ang);
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
			purepursuit::kD_ang += 0.001;
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
			purepursuit::kD_ang -= 0.001;
		break;
	}
}

okapi::Point
get_intersection(std::vector<okapi::Point> path, okapi::QLength radius)
{
	okapi::QLength x1, x2, x3, y1, y2, y3;
	okapi::QLength x_int_1, x_int_2, y_int_1, y_int_2;
	std::vector<okapi::Point> intersection_points;
	okapi::Point last_point;
	okapi::Point second_last_point;
	okapi::Point target_point;

	x1 = drive->getState().x;
	y1 = drive->getState().y;

	while (intersection_points.size() == 0) {
		for (int i = path.size() - 1; i >= 0; i--) {
			x2 = path[i - 1].x;
			y2 = path[i - 1].y;
			x3 = path[i].x;
			y3 = path[i].y;

			auto m = (y3 - y2) / (x3 - x2);
			if (isfinite(m.getValue())) {
				auto a = x1 + m * m * x2 - m * y2 + m * y1;
				auto b = sqrt(
					-m * m * x1 * x1 + 2 * m * m * x1 * x2 +
					2 * m * x1 * y1 - 2 * m * x1 * y2 +
					m * m * radius * radius +
					2 * m * x2 * y2 + radius * radius +
					2 * y2 * y1 - m * m * x2 * x2 -
					2 * m * x2 * y1 - y2 * y2 - y1 * y1);
				auto c = m / m + m * m;

				x_int_1 = (a - b) / c;
				x_int_2 = (a + b) / c;

				if (isfinite(x_int_1.getValue())) {
					if ((x2 < x3 && x_int_1 < x3 &&
					     x_int_1 > x2) ||
					    (x2 > x3 && x_int_1 > x3 &&
					     x_int_1 < x2)) {
						y_int_1 =
							m * (x_int_1 - x2) + y2;
						intersection_points.push_back(
							{ x_int_1, y_int_1 });
					}
					if ((x2 < x3 && x_int_2 < x3 &&
					     x_int_2 > x2) ||
					    (x2 > x3 && x_int_2 > x3 &&
					     x_int_2 < x2)) {
						y_int_2 =
							m * (x_int_2 - x2) + y2;
						intersection_points.push_back(
							{ x_int_2, y_int_2 });
					}
				}
			} else {
				auto a = sqrt(radius * radius - x2 * x2 +
					      2 * x2 * x1 - x1 * x1);
				y_int_1 = x1 - a;
				y_int_2 = x1 + a;
				x_int_1 = x2;

				if (isfinite(y_int_1.getValue())) {
					if ((x2 < x3 && x_int_1 < x3 &&
					     x_int_1 > x2) ||
					    (x2 > x3 && x_int_1 > x3 &&
					     x_int_1 < x2))
						intersection_points.push_back(
							{ x_int_1, y_int_1 });
					if ((x2 < x3 && x_int_2 < x3 &&
					     x_int_2 > x2) ||
					    (x2 > x3 && x_int_2 > x3 &&
					     x_int_2 < x2))
						intersection_points.push_back(
							{ x_int_1, y_int_2 });
				}
			}
			if (intersection_points.size() != 0) {
				last_segment = i == path.size() - 1;
				break;
			}
		}
		radius += 0.5_in;
	}

	last_point = intersection_points.at(0);

	if (intersection_points.size() == 1) {
		target_point = last_point;
	} else {
		second_last_point = intersection_points.at(1);
		if (x3 > x2) {
			if (last_point.x > second_last_point.x)
				target_point = last_point;
			else
				target_point = second_last_point;
		} else if (x2 > x3) {
			if (last_point.x > second_last_point.x)
				target_point = second_last_point;
			else
				target_point = last_point;
		} else {
			if (y3 > y2) {
				if (last_point.y > second_last_point.y)
					target_point = last_point;
				else
					target_point = second_last_point;
			} else if (y2 > y3) {
				if (last_point.y > second_last_point.y)
					target_point = second_last_point;
				else
					target_point = last_point;
			} else
				target_point = last_point;
		}
	}

	return target_point;
}

void
follow_path(std::vector<okapi::Point> path)
{
	double max_speed = 80;

	okapi::QLength inner_radius = 15.0_in;
	okapi::QLength outer_radius = 15.0_in;

	double kP_ang = 0.08;
	double kD_ang = 0.08;

	double kP_vel = 0.08;
	double kD_vel = 0.08;

	okapi::QAngle ang_prev_error = 0_deg;
	okapi::QAngle vel_prev_error = 0_deg;

	double left_prev = 0;
	double right_prev = 0;

	for (;;) {
		okapi::Point ang_tracking_point =
			get_intersection(path, inner_radius);
		if (last_segment) {
			drive->driveToPoint(path[path.size() - 1]);
			break;
		}
		okapi::Point vel_tracking_point =
			get_intersection(path, outer_radius);

		okapi::QAngle ang_error =
			odom::get_delta_theta(ang_tracking_point);
		okapi::QAngle vel_error =
			odom::get_delta_theta(vel_tracking_point);

		okapi::QAngle ang_derivative = ang_error - ang_prev_error;
		okapi::QAngle vel_derivative = vel_error - vel_prev_error;

		ang_prev_error = ang_error;
		vel_prev_error = vel_error;

		double forward_speed;
		if (vel_error.getValue() == 0) {
			forward_speed = max_speed;
		} else {
			forward_speed = kP_vel / vel_error.getValue() -
					kD_vel * vel_derivative.getValue();
		}
		double turn_modifier = kP_ang * ang_error.getValue() +
				       kD_ang * ang_derivative.getValue();

		double left_speed = -(forward_speed + turn_modifier);
		double right_speed = -(forward_speed - turn_modifier);

		if (left_speed > max_speed) {
			double diff = left_speed - max_speed;
			left_speed -= diff;
			right_speed -= diff;
		} else if (left_speed < -max_speed) {
			double diff = left_speed + max_speed;
			left_speed -= diff;
			right_speed -= diff;
		}

		if (right_speed > max_speed) {
			double diff = right_speed - max_speed;
			left_speed -= diff;
			right_speed -= diff;
		} else if (right_speed < -max_speed) {
			double diff = right_speed + max_speed;
			left_speed -= diff;
			right_speed -= diff;
		}

		left_speed = util::slew(&left_prev, 5, left_speed);
		right_speed = util::slew(&right_prev, 5, right_speed);

		left_prev = left_speed;
		right_prev = right_speed;

		drive->getModel()->left(left_speed);
		drive->getModel()->right(right_speed);

		pros::delay(10);
	}
}

} // namespace purepursuit
