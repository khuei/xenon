#include "pros/rtos.hpp"

#include "config.h"
#include "logger.h"

namespace chassis {

enum tune_value {
	MAX_SPEED,
	DISTANCE_CONSTANT,
	TURN_CONSTANT,
};

int current_tune_value = 0;

double max_speed = 100;
okapi::QLength distance_constant = 3.85_in;
okapi::QLength turn_constant = 22.75_in;

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
			current_tune_value = TURN_CONSTANT;
	}

	if (master.get_digital_new_press(right)) {
		master.clear_line(2);
		if (current_tune_value != TURN_CONSTANT)
			current_tune_value++;
		else
			current_tune_value = MAX_SPEED;
	}

	if (master.get_digital(left) || master.get_digital(right) || master.get_digital(decrease) ||
	    master.get_digital(increase)) {
		switch (current_tune_value) {
		case MAX_SPEED:
			master.print(2, 0, "max_speed: %f", chassis::max_speed);
			if (master.get_digital_new_press(increase))
				chassis::max_speed += 2;
			if (master.get_digital_new_press(decrease))
				chassis::max_speed -= 2;
			break;
		case DISTANCE_CONSTANT:
			master.print(2,
				     0,
				     "dist_const: %s",
				     std::to_string(chassis::distance_constant.convert(okapi::inch)));
			if (master.get_digital_new_press(increase))
				chassis::distance_constant += 0.01_in;
			if (master.get_digital_new_press(decrease))
				chassis::distance_constant -= 0.01_in;
			break;
		case TURN_CONSTANT:
			master.print(2,
				     0,
				     "turn_const: %s",
				     std::to_string(chassis::turn_constant.convert(okapi::inch)));
			if (master.get_digital_new_press(increase))
				chassis::turn_constant += 0.01_in;
			if (master.get_digital_new_press(decrease))
				chassis::turn_constant -= 0.01_in;
			break;
		}
	}
}

} // namespace chassis

namespace intake {

enum tune_value {
	MAX_SPEED,
	ACCEL_STEP,
};

int current_tune_value = 0;

double max_speed = 200;
double accel_step = 5;

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
			current_tune_value = ACCEL_STEP;
	}

	if (master.get_digital_new_press(right)) {
		master.clear_line(2);
		if (current_tune_value != ACCEL_STEP)
			current_tune_value++;
		else
			current_tune_value = MAX_SPEED;
	}


	if (master.get_digital(left) || master.get_digital(right) || master.get_digital(decrease) ||
	    master.get_digital(increase)) {
		switch(current_tune_value) {
		case MAX_SPEED:
			master.print(2, 0, "max_speed: %f", intake::max_speed);
			if (master.get_digital_new_press(increase))
				intake::max_speed += 2;
			if (master.get_digital_new_press(decrease))
				intake::max_speed -= 2;
			break;
		case ACCEL_STEP:
			master.print(2, 0, "accel_step: %f", intake::accel_step);
			if (master.get_digital_new_press(increase))
				intake::accel_step += 1;
			if (master.get_digital_new_press(decrease))
				intake::accel_step -= 1;
			break;
		}
	}
}

} // namespace intake

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

	if (master.get_digital(left) || master.get_digital(right) || master.get_digital(decrease) ||
	    master.get_digital(increase)) {
		switch (current_tune_value) {
		case MAX_SPEED:
			master.print(2, 0, "max_speed: %f", purepursuit::max_speed);
			if (master.get_digital_new_press(increase))
				purepursuit::max_speed += 2;
			if (master.get_digital_new_press(decrease))
				purepursuit::max_speed -= 2;
			break;
		case ACCEL_STEP:
			master.print(2, 0, "accel_step: %f", purepursuit::accel_step);
			if (master.get_digital_new_press(increase))
				purepursuit::accel_step += 1;
			if (master.get_digital_new_press(decrease))
				purepursuit::accel_step -= 1;
			break;
		case INNER_RADIUS:
			master.print(2,
				     0,
				     "inner_radius: %s",
				     std::to_string(purepursuit::inner_radius.convert(okapi::inch)));
			if (master.get_digital_new_press(increase))
				purepursuit::inner_radius += 1_in;
			if (master.get_digital_new_press(decrease))
				purepursuit::inner_radius -= 1_in;
			break;
		case OUTER_RADIUS:
			master.print(2,
				     0,
				     "outer_radius: %s",
				     std::to_string(purepursuit::outer_radius.convert(okapi::inch)));
			if (master.get_digital_new_press(increase))
				purepursuit::outer_radius += 1_in;
			if (master.get_digital_new_press(decrease))
				purepursuit::outer_radius -= 1_in;
			break;
		case KP_VEL:
			master.print(2, 0, "kP_vel: %f", purepursuit::kP_vel);
			if (master.get_digital_new_press(increase))
				purepursuit::kP_vel += 0.001;
			if (master.get_digital_new_press(decrease))
				purepursuit::kP_vel -= 0.001;
			break;
		case KD_VEL:
			master.print(2, 0, "kD_vel: %f", purepursuit::kD_vel);
			if (master.get_digital_new_press(increase))
				purepursuit::kD_vel += 0.001;
			if (master.get_digital_new_press(decrease))
				purepursuit::kD_vel -= 0.001;
			break;
		case KP_ANG:
			master.print(2, 0, "kP_ang: %f", purepursuit::kP_ang);
			if (master.get_digital_new_press(increase))
				purepursuit::kP_ang += 0.001;
			if (master.get_digital_new_press(decrease))
				purepursuit::kP_ang -= 0.001;
			break;
		case KD_ANG:
			master.print(2, 0, "kD_ang: %f", purepursuit::kD_ang);
			if (master.get_digital_new_press(increase))
				purepursuit::kD_ang += 0.001;
			if (master.get_digital_new_press(decrease))
				purepursuit::kD_ang -= 0.001;
			break;
		}
	}
}

} // namespace purepursuit

namespace config {

enum tuner_mode {
	CHASSIS_TUNER,
	INTAKE_TUNER,
	PUREPURSUIT_TUNER,
};

int current_tuner = 0;

bool started;

void
switch_tuner(pros::controller_digital_e_t left, pros::controller_digital_e_t right)
{
	if (master.get_digital_new_press(left)) {
		master.clear_line(2);
		if (current_tuner != CHASSIS_TUNER)
			current_tuner--;
		else
			current_tuner = PUREPURSUIT_TUNER;
	}

	if (master.get_digital_new_press(right)) {
		master.clear_line(2);
		if (current_tuner != PUREPURSUIT_TUNER)
			current_tuner++;
		else
			current_tuner = CHASSIS_TUNER;
	}

	if (master.get_digital(left) || master.get_digital(right)) {
		switch (current_tuner) {
		case CHASSIS_TUNER:
			master.print(2, 0, "chassis tuner");
			break;
		case INTAKE_TUNER:
			master.print(2, 0, "intake tuner");
			break;
		case PUREPURSUIT_TUNER:
			master.print(2, 0, "purepursuit tuner");
			break;
		}
	}
}

void
switch_tab(pros::controller_digital_e_t left, pros::controller_digital_e_t right)
{
}

void
init(void)
{
}

void
stop(void)
{
	started = false;
}

} // namespace tuner
