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

double max_speed = 200;
okapi::QLength distance_constant = 4.25_in;
okapi::QLength turn_constant = 21.50_in;

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
		switch (current_tune_value) {
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

enum tab {
	CHASSIS_TAB,
	INTAKE_TAB,
	PUREPURSUIT_TAB,
};

int current_tab;

lv_obj_t *tabview;

lv_obj_t *chassis_tab;
lv_obj_t *chassis_btnm;
const char *chassis_btnm_map[8] = { "SPD" SYMBOL_PLUS,	"D_CST" SYMBOL_PLUS,  "T_CST" SYMBOL_PLUS,  "\n",
				    "SPD" SYMBOL_MINUS, "D_CST" SYMBOL_MINUS, "T_CST" SYMBOL_MINUS, "" };
lv_obj_t *chassis_label;

lv_obj_t *intake_tab;
lv_obj_t *intake_btnm;
const char *intake_btnm_map[6] = { "SPD" SYMBOL_PLUS,  "ACCEL" SYMBOL_PLUS,  "\n",
				   "SPD" SYMBOL_MINUS, "ACCEL" SYMBOL_MINUS, "" };
lv_obj_t *intake_label;

lv_obj_t *purepursuit_tab;
lv_obj_t *purepursuit_btnm;
const char *purepursuit_btnm_map[20] = {
	"SPD" SYMBOL_PLUS,     "ACCEL" SYMBOL_PLUS,   "I_RAD" SYMBOL_PLUS,   "O_RAD" SYMBOL_PLUS,   "\n",
	"SPD" SYMBOL_MINUS,    "ACCEL" SYMBOL_MINUS,  "I_RAD" SYMBOL_MINUS,  "O_RAD" SYMBOL_MINUS,  "\n",
	"kP_VEL" SYMBOL_PLUS,  "kD_VEL" SYMBOL_PLUS,  "kP_ANG" SYMBOL_PLUS,  "kD_ANG" SYMBOL_PLUS,  "\n",
	"kP_VEL" SYMBOL_MINUS, "kD_VEL" SYMBOL_MINUS, "kP_ANG" SYMBOL_MINUS, "kD_ANG" SYMBOL_MINUS, ""
};
lv_obj_t *purepursuit_misc_label;
lv_obj_t *purepursuit_pid_label;

bool started;

std::shared_ptr<pros::Task> configTask;

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
tuner(void)
{
	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X))
		config::switch_tuner(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);

	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
		switch (config::current_tuner) {
		case 0:
			chassis::tuner(pros::E_CONTROLLER_DIGITAL_LEFT,
				       pros::E_CONTROLLER_DIGITAL_RIGHT,
				       pros::E_CONTROLLER_DIGITAL_UP,
				       pros::E_CONTROLLER_DIGITAL_DOWN);
			break;
		case 1:
			intake::tuner(pros::E_CONTROLLER_DIGITAL_LEFT,
				      pros::E_CONTROLLER_DIGITAL_RIGHT,
				      pros::E_CONTROLLER_DIGITAL_UP,
				      pros::E_CONTROLLER_DIGITAL_DOWN);
			break;
		case 2:
			purepursuit::tuner(pros::E_CONTROLLER_DIGITAL_LEFT,
					   pros::E_CONTROLLER_DIGITAL_RIGHT,
					   pros::E_CONTROLLER_DIGITAL_UP,
					   pros::E_CONTROLLER_DIGITAL_DOWN);
			break;
		}
	}
}

void
switch_tab(pros::controller_digital_e_t left, pros::controller_digital_e_t right)
{
	if (!config::started)
		return;

	if (master.get_digital_new_press(left)) {
		if (current_tab > CHASSIS_TAB)
			current_tab--;
		else
			current_tab = PUREPURSUIT_TAB;
	}

	if (master.get_digital_new_press(right)) {
		if (current_tab < PUREPURSUIT_TAB)
			current_tab++;
		else
			current_tab = CHASSIS_TAB;
	}

	if (master.get_digital(left) || master.get_digital(right)) {
		switch (current_tab) {
		case CHASSIS_TAB:
			lv_tabview_set_tab_act(tabview, CHASSIS_TAB, false);
			break;
		case INTAKE_TAB:
			lv_tabview_set_tab_act(tabview, INTAKE_TAB, false);
			break;
		case PUREPURSUIT_TAB:
			lv_tabview_set_tab_act(tabview, PUREPURSUIT_TAB, false);
			break;
		}
	}
}

int
config_task(void)
{
	std::uint32_t now = pros::millis();
	for (;;) {
		/*            CHASSIS TAB            */

		std::string chassis_label_str = std::to_string(chassis::max_speed) + "         " +
						std::to_string(chassis::distance_constant.convert(okapi::inch)) +
						"          " +
						std::to_string(chassis::turn_constant.convert(okapi::inch));
		lv_label_set_text(chassis_label, chassis_label_str.c_str());

		/*            INTAKE TAB            */

		std::string intake_label_str =
			std::to_string(intake::max_speed) + "                    " + std::to_string(intake::accel_step);
		lv_label_set_text(intake_label, intake_label_str.c_str());

		/*            PUREPURSUIT TAB            */

		std::string purepursuit_misc_label_str =
			std::to_string(purepursuit::max_speed) + "     " + std::to_string(purepursuit::accel_step) +
			"     " + std::to_string(purepursuit::inner_radius.convert(okapi::inch)) + "     " +
			std::to_string(purepursuit::outer_radius.convert(okapi::inch));
		lv_label_set_text(purepursuit_misc_label, purepursuit_misc_label_str.c_str());

		std::string purepursuit_pid_label_str = std::to_string(purepursuit::kP_vel) + "      " +
							std::to_string(purepursuit::kD_vel) + "      " +
							std::to_string(purepursuit::kP_ang) + "      " +
							std::to_string(purepursuit::kD_ang) + "      ";
		lv_label_set_text(purepursuit_pid_label, purepursuit_pid_label_str.c_str());

		pros::Task::delay_until(&now, 100);
	}
}

lv_res_t
chassis_btnm_action(lv_obj_t *btnm, const char *label)
{
	if (strcmp(label, "SPD" SYMBOL_PLUS) == 0)
		chassis::max_speed += 2;
	if (strcmp(label, "SPD" SYMBOL_MINUS) == 0)
		chassis::max_speed -= 2;

	if (strcmp(label, "D_CST" SYMBOL_PLUS) == 0)
		chassis::distance_constant += 0.01_in;
	if (strcmp(label, "D_CST" SYMBOL_MINUS) == 0)
		chassis::distance_constant -= 0.01_in;

	if (strcmp(label, "T_CST" SYMBOL_PLUS) == 0)
		chassis::turn_constant += 0.01_in;
	if (strcmp(label, "T_CST" SYMBOL_MINUS) == 0)
		chassis::turn_constant -= 0.01_in;

	return LV_RES_OK;
}

lv_res_t
intake_btnm_action(lv_obj_t *btnm, const char *label)
{
	if (strcmp(label, "SPD" SYMBOL_PLUS) == 0)
		intake::max_speed += 2;
	if (strcmp(label, "SPD" SYMBOL_MINUS) == 0)
		intake::max_speed -= 2;

	if (strcmp(label, "ACCEL" SYMBOL_PLUS) == 0)
		intake::accel_step += 1;
	if (strcmp(label, "ACCEL" SYMBOL_MINUS) == 0)
		intake::accel_step -= 1;

	return LV_RES_OK;
}

lv_res_t
purepursuit_btnm_action(lv_obj_t *btnm, const char *label)
{
	if (strcmp(label, "SPD" SYMBOL_PLUS) == 0)
		purepursuit::max_speed += 2;
	if (strcmp(label, "SPD" SYMBOL_MINUS) == 0)
		purepursuit::max_speed -= 2;

	if (strcmp(label, "ACCEL" SYMBOL_PLUS) == 0)
		purepursuit::accel_step += 1;
	if (strcmp(label, "ACCEL" SYMBOL_MINUS) == 0)
		purepursuit::accel_step -= 1;

	if (strcmp(label, "I_RAD" SYMBOL_PLUS) == 0)
		purepursuit::inner_radius += 1_in;
	if (strcmp(label, "I_RAD" SYMBOL_MINUS) == 0)
		purepursuit::inner_radius -= 1_in;

	if (strcmp(label, "O_RAD" SYMBOL_PLUS) == 0)
		purepursuit::outer_radius += 1_in;
	if (strcmp(label, "O_RAD" SYMBOL_MINUS) == 0)
		purepursuit::outer_radius -= 1_in;

	if (strcmp(label, "kP_VEL" SYMBOL_PLUS) == 0)
		purepursuit::kP_vel += 0.001;
	if (strcmp(label, "kP_VEL" SYMBOL_MINUS) == 0)
		purepursuit::kP_vel -= 0.001;

	if (strcmp(label, "kD_VEL" SYMBOL_PLUS) == 0)
		purepursuit::kD_vel += 0.001;
	if (strcmp(label, "kD_VEL" SYMBOL_MINUS) == 0)
		purepursuit::kD_vel -= 0.001;

	if (strcmp(label, "kP_ANG" SYMBOL_PLUS) == 0)
		purepursuit::kP_ang += 0.001;
	if (strcmp(label, "kP_ANG" SYMBOL_MINUS) == 0)
		purepursuit::kP_ang -= 0.001;

	if (strcmp(label, "kD_ANG" SYMBOL_PLUS) == 0)
		purepursuit::kD_ang += 0.001;
	if (strcmp(label, "kD_ANG" SYMBOL_MINUS) == 0)
		purepursuit::kD_ang -= 0.001;

	return LV_RES_OK;
}

void
init(void)
{
	/*            MAIN TAB            */

	tabview = lv_tabview_create(lv_scr_act(), NULL);

	lv_tabview_set_anim_time(tabview, 0);
	lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_INDIC, &lv_style_transp);

	logger::elog("config: create main tab");

	/*            CHASSIS TAB            */

	chassis_tab = lv_tabview_add_tab(tabview, "chassis");
	lv_page_set_sb_mode(chassis_tab, LV_SB_MODE_OFF);

	chassis_btnm = lv_btnm_create(chassis_tab, NULL);
	lv_btnm_set_map(chassis_btnm, chassis_btnm_map);
	lv_btnm_set_action(chassis_btnm, chassis_btnm_action);
	lv_obj_set_size(chassis_btnm, LV_HOR_RES * 0.95, LV_VER_RES * 0.25);
	lv_obj_align(chassis_btnm, NULL, LV_ALIGN_CENTER, 0, 0);

	chassis_label = lv_label_create(chassis_tab, NULL);
	lv_obj_align(chassis_label, NULL, LV_ALIGN_IN_LEFT_MID, 40, 4);

	logger::elog("config: create chassis tab");

	/*            INTAKE TAB            */

	intake_tab = lv_tabview_add_tab(tabview, "intake");
	lv_page_set_sb_mode(intake_tab, LV_SB_MODE_OFF);

	intake_btnm = lv_btnm_create(intake_tab, NULL);
	lv_btnm_set_map(intake_btnm, intake_btnm_map);
	lv_btnm_set_action(intake_btnm, intake_btnm_action);
	lv_obj_set_size(intake_btnm, LV_HOR_RES * 0.95, LV_VER_RES * 0.25);
	lv_obj_align(intake_btnm, NULL, LV_ALIGN_CENTER, 0, 0);

	intake_label = lv_label_create(intake_tab, NULL);
	lv_obj_align(intake_label, NULL, LV_ALIGN_IN_LEFT_MID, 70, 4);

	logger::elog("config: create intake tab");

	/*            PUREPURSUIT TAB            */

	purepursuit_tab = lv_tabview_add_tab(tabview, "purepursuit");
	lv_page_set_sb_mode(purepursuit_tab, LV_SB_MODE_OFF);

	purepursuit_btnm = lv_btnm_create(purepursuit_tab, NULL);
	lv_btnm_set_map(purepursuit_btnm, purepursuit_btnm_map);
	lv_btnm_set_action(purepursuit_btnm, purepursuit_btnm_action);
	lv_obj_set_size(purepursuit_btnm, LV_HOR_RES * 0.95, LV_VER_RES * 0.6);
	lv_obj_align(purepursuit_btnm, NULL, LV_ALIGN_CENTER, 0, 0);

	purepursuit_misc_label = lv_label_create(purepursuit_tab, NULL);
	lv_obj_align(purepursuit_misc_label, NULL, LV_ALIGN_IN_LEFT_MID, 20, -30);

	purepursuit_pid_label = lv_label_create(purepursuit_tab, NULL);
	lv_obj_align(purepursuit_pid_label, NULL, LV_ALIGN_IN_LEFT_MID, 20, 40);

	logger::elog("config: create purepursuit tab");

	/*            TASK            */

	if (!configTask) {
		configTask = std::make_shared<pros::Task>(config_task, "config task (xenon)");
		configTask->set_priority(TASK_PRIORITY_MIN);
		logger::elog("config: create task (xenon)");
	} else {
		configTask->resume();
		logger::elog("config: resume task (xenon)");
	}

	started = true;
}

void
stop(void)
{
	lv_obj_clean(lv_scr_act());
	logger::elog("config: clean screen");

	configTask->suspend();
	logger::elog("config: suspend task (xenon)");

	started = false;
}

} // namespace tuner
