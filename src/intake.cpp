#include "intake.h"
#include "util.h"
#include "logger.h"
#include "config.h"

namespace intake {

double front_speed = 0;
double internal_speed = 0;

void
reset(void)
{
	left_intake->tarePosition();
	right_intake->tarePosition();
	top_intake->tarePosition();
	bottom_intake->tarePosition();
}

void
set_brake(okapi::AbstractMotor::brakeMode brake_mode)
{
	left_intake->setBrakeMode(brake_mode);
	right_intake->setBrakeMode(brake_mode);
	top_intake->setBrakeMode(brake_mode);
	bottom_intake->setBrakeMode(brake_mode);
}

void
run_front(double max_speed)
{
	front_speed = util::slew(&front_speed, accel_step, max_speed);
	front_model->forward(front_speed);
}

void
run_internal(double max_speed)
{
	internal_speed = util::slew(&internal_speed, accel_step, max_speed);
	internal_model->forward(internal_speed);
}

void
run_both(double speed)
{
	run_front(speed);
	run_internal(speed);
}

void
eject(double max_speed)
{
	internal_speed = util::slew(&internal_speed, accel_step, max_speed);
	internal_model->left(internal_speed);
	internal_model->right(-internal_speed);
}

void
sort(double speed)
{
	int target_low_limit;
	int target_high_limit;
	int eject_low_limit;
	int eject_high_limit;

	if (auton::alliance_color.compare("blue") == 0) {
		target_low_limit = 230;
		target_high_limit = 250;
		eject_low_limit = 10;
		eject_high_limit = 30;
	} else if (auton::alliance_color.compare("red") == 0) {
		target_low_limit = 10;
		target_high_limit = 30;
		eject_low_limit = 230;
		eject_high_limit = 250;
	}

	if (vision->get_object_count() == 0 &&
	    !master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
		internal_model->right(0);

	if (optical->get_hue() > target_low_limit && optical->get_hue() < target_high_limit) {
		if (vision->get_object_count() != 0) {
			internal_model->left(speed);
			internal_model->right(speed);
		} else {
			internal_model->left(0);
			internal_model->right(0);
		}
	} else if (optical->get_hue() > eject_low_limit && optical->get_hue() < eject_high_limit){
		if (vision->get_object_count() != 0) {
			internal_model->right(-speed);
			internal_model->left(speed);
		} else {
			internal_model->right(0);
			internal_model->left(speed);
		}
	}
}

void
init(void)
{
	intake::reset();
	logger::elog("intake: reset");

	intake::set_brake(okapi::AbstractMotor::brakeMode::hold);
	logger::elog("intake: set brake mode to hold");

	optical->set_led_pwm(100);

	vision->clear_led();
	light->set_led(COLOR_RED);
	vision->set_auto_white_balance(false);
	vision->set_exposure(150);

	pros::vision_signature_s_t RED_SIG = pros::Vision::signature_from_utility(1, 8973, 11143, 10058, -2119, -1053, -1586, 5.4, 0);
	vision->set_signature(1, &RED_SIG);

	front_intake->setMaxVelocity(intake::max_speed);
	internal_intake->setMaxVelocity(intake::max_speed);
}

void
opcontrol(void)
{
	if (!intake::baby_mode) {
		switch (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
		case 0:
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
			    master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::eject(intake::max_speed);
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
				 !master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_internal(intake::max_speed);
			else
				intake::run_internal(0);

			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
			    master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_front(-intake::max_speed);
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
				 !master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
				intake::run_front(intake::max_speed);
			else
				intake::run_front(0);
			break;
		case 1:
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
				intake::run_both(-intake::max_speed);
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
				intake::run_both(intake::max_speed);
			else
				intake::run_both(0);
			break;
		}
	} else {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
			intake::run_front(intake::max_speed);
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
			intake::run_front(-intake::max_speed);
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
			intake::eject(intake::max_speed);
		else
			intake::run_front(0);

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
			intake::run_internal(intake::max_speed);
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
			intake::run_internal(-intake::max_speed);
		else
			intake::run_internal(0);
	}

	if (intake::auto_sort) {
		intake::sort(intake::max_speed);
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
			internal_model->right(intake::max_speed);
	}

}

} // namespace intake
