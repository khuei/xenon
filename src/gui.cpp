#include <iostream>

#include "pros/rtos.hpp"

#include "gui.h"
#include "checkhealth.h"
#include "logger.h"

namespace gui {

enum gui_tab {
	CHASSIS_TAB,
	INTAKE_TAB,
	ODOM_TAB,
	HEALTH_TAB,
	LOG_TAB,
};

int current_tab = 0;

lv_obj_t *tabview;

lv_obj_t *chassis_tab;
lv_obj_t *chassis_left_speed_gauge;
lv_obj_t *chassis_right_speed_gauge;
lv_obj_t *chassis_left_label;
lv_obj_t *chassis_right_label;

lv_obj_t *intake_tab;
lv_obj_t *intake_front_gauge;
lv_obj_t *intake_internal_gauge;
lv_obj_t *intake_front_label;
lv_obj_t *intake_internal_label;

lv_obj_t *health_tab;
lv_obj_t *chassis_health_label;
lv_obj_t *intake_health_label;
lv_obj_t *core_health_label;

lv_obj_t *odom_tab;
lv_obj_t *odom_label;

lv_obj_t *log_tab;
lv_obj_t *log_stream;

bool started;

std::shared_ptr<pros::Task> guiTask;

int
gui_task(void)
{
	for (;;) {
		/*            CHASSIS TAB            */

		double chassis_left_speed = (front_left->getActualVelocity() + back_left->getActualVelocity()) / 2;
		double chassis_right_speed = (front_right->getActualVelocity() + back_right->getActualVelocity()) / 2;

		double chassis_left_pos = (front_left->getPosition() + back_left->getPosition()) / 2;
		double chassis_right_pos = (front_right->getPosition() + back_right->getPosition()) / 2;

		lv_gauge_set_value(chassis_left_speed_gauge, 0, fabs(chassis_left_speed));
		lv_gauge_set_value(chassis_right_speed_gauge, 0, fabs(chassis_right_speed));

		std::string chassis_left_speed_str =
			std::to_string(chassis_left_speed) + " RPM\n" + std::to_string(chassis_left_pos) + " DEG\n";
		std::string chassis_right_speed_str =
			std::to_string(chassis_right_speed) + " RPM\n" + std::to_string(chassis_right_pos) + " DEG\n";

		lv_label_set_text(chassis_left_label, chassis_left_speed_str.c_str());
		lv_label_set_text(chassis_right_label, chassis_right_speed_str.c_str());

		/*            INTAKE TAB            */

		double intake_front_speed = (left_intake->getActualVelocity() + right_intake->getActualVelocity()) / 2;
		double intake_internal_speed =
			(top_intake->getActualVelocity() + bottom_intake->getActualVelocity()) / 2;

		double intake_front_pos = (left_intake->getPosition() + right_intake->getPosition()) / 2;
		double intake_internal_pos = (top_intake->getPosition() + bottom_intake->getPosition()) / 2;

		std::string intake_front_str =
			std::to_string(intake_front_speed) + " RPM\n" + std::to_string(intake_front_pos) + " DEG\n";
		std::string intake_internal_str = std::to_string(intake_internal_speed) + " RPM\n" +
						  std::to_string(intake_internal_pos) + " DEG\n";

		lv_label_set_text(intake_front_label, intake_front_str.c_str());
		lv_label_set_text(intake_internal_label, intake_internal_str.c_str());

		/*            ODOM TAB            */

		std::string pos = "global_x: " + std::to_string(drive->getState().x.convert(okapi::inch)) + "_in\n" +
				  "global_y: " + std::to_string(drive->getState().y.convert(okapi::inch)) + "_in\n" +
				  "global_theta: " + std::to_string(drive->getState().theta.convert(okapi::degree)) +
				  "_deg\n";
		lv_label_set_text(odom_label, pos.c_str());

		/*            HEALTH TAB            */

		std::string chassis_health_str = "Chassis\n\nfront left: " + checkhealth::motor(front_left) +
						 "\n" + "front right: " + checkhealth::motor(front_right) +
						 "\n" + "back left: " + checkhealth::motor(back_left) +
						 "\n" + "back right: " + checkhealth::motor(back_right);

		lv_label_set_text(chassis_health_label, chassis_health_str.c_str());

		std::string intake_health_str = "Intake\n\nleft: " + checkhealth::motor(left_intake) +
						 "\n" + "right: " + checkhealth::motor(right_intake) +
						 "\n" + "top: " + checkhealth::motor(top_intake) +
						 "\n" + "bot: " + checkhealth::motor(bottom_intake);

		lv_label_set_text(intake_health_label, intake_health_str.c_str());

		std::string core_health_str = "Core\n\ncontroller: " + checkhealth::controller(master) + "\n"
			+ "battery: " + checkhealth::battery() + "\n"
			+ "usd: " + checkhealth::usd();

		lv_label_set_text(core_health_label, core_health_str.c_str());

		/*            LOG TAB            */

		lv_label_set_text(log_stream, logger::ebuf());

		pros::delay(1000);
	}
}

void
switch_tab(pros::controller_digital_e_t left, pros::controller_digital_e_t right)
{
	if (master.get_digital_new_press(left)) {
		if (current_tab > CHASSIS_TAB)
			current_tab--;
		else
			current_tab = LOG_TAB;
	}

	if (master.get_digital_new_press(right)) {
		if (current_tab < LOG_TAB)
			current_tab++;
		else
			current_tab = CHASSIS_TAB;
	}

	switch (current_tab) {
	case CHASSIS_TAB:
		lv_tabview_set_tab_act(tabview, CHASSIS_TAB, false);
		break;
	case INTAKE_TAB:
		lv_tabview_set_tab_act(tabview, INTAKE_TAB, false);
		break;
	case ODOM_TAB:
		lv_tabview_set_tab_act(tabview, ODOM_TAB, false);
		break;
	case HEALTH_TAB:
		lv_tabview_set_tab_act(tabview, HEALTH_TAB, false);
		break;
	case LOG_TAB:
		lv_tabview_set_tab_act(tabview, LOG_TAB, false);
		break;
	}
}

void
init(void)
{
	/*            MAIN TAB            */

	tabview = lv_tabview_create(lv_scr_act(), NULL);

	logger::elog("gui: create main tab");

	/*            CHASSIS TAB            */

	chassis_tab = lv_tabview_add_tab(tabview, "chassis");

	chassis_left_speed_gauge = lv_gauge_create(chassis_tab, NULL);
	lv_obj_set_size(chassis_left_speed_gauge, 180, 150);
	lv_obj_align(chassis_left_speed_gauge, NULL, LV_ALIGN_IN_LEFT_MID, 20, 0);
	chassis_left_label = lv_label_create(chassis_tab, NULL);
	lv_obj_align(chassis_left_label, NULL, LV_ALIGN_CENTER, -150, 50);

	chassis_right_speed_gauge = lv_gauge_create(chassis_tab, NULL);
	lv_obj_set_size(chassis_right_speed_gauge, 180, 150);
	lv_obj_align(chassis_right_speed_gauge, NULL, LV_ALIGN_IN_RIGHT_MID, -20, 0);
	chassis_right_label = lv_label_create(chassis_tab, NULL);
	lv_obj_align(chassis_right_label, NULL, LV_ALIGN_CENTER, 90, 50);

	logger::elog("gui: create chassis tab");

	/*            INTAKE TAB            */

	intake_tab = lv_tabview_add_tab(tabview, "intake");

	intake_front_gauge = lv_gauge_create(intake_tab, NULL);
	lv_gauge_set_range(intake_front_gauge, 0, 200);
	lv_obj_set_size(intake_front_gauge, 180, 150);
	lv_obj_align(intake_front_gauge, NULL, LV_ALIGN_IN_LEFT_MID, 20, 0);
	intake_front_label = lv_label_create(intake_tab, NULL);
	lv_obj_align(intake_front_label, NULL, LV_ALIGN_CENTER, -150, 50);

	intake_internal_gauge = lv_gauge_create(intake_tab, NULL);
	lv_gauge_set_range(intake_internal_gauge, 0, 200);
	lv_obj_set_size(intake_internal_gauge, 180, 150);
	lv_obj_align(intake_internal_gauge, NULL, LV_ALIGN_IN_RIGHT_MID, -20, 0);
	intake_internal_label = lv_label_create(intake_tab, NULL);
	lv_obj_align(intake_internal_label, NULL, LV_ALIGN_CENTER, 90, 50);

	logger::elog("gui: create intake tab");

	/*            ODOM TAB            */

	odom_tab = lv_tabview_add_tab(tabview, "odom");
	odom_label = lv_label_create(odom_tab, NULL);

	logger::elog("gui: create odom tab");

	/*            HEALTH TAB            */

	health_tab = lv_tabview_add_tab(tabview, "health");

	chassis_health_label = lv_label_create(health_tab, NULL);
	lv_obj_align(chassis_health_label, NULL, LV_ALIGN_CENTER, -200, 0);

	intake_health_label = lv_label_create(health_tab, NULL);
	lv_obj_align(intake_health_label, NULL, LV_ALIGN_CENTER, -35, 0);

	core_health_label = lv_label_create(health_tab, NULL);
	lv_obj_align(core_health_label, NULL, LV_ALIGN_CENTER, 120, 0);

	logger::elog("gui: create health tab");

	/*            LOG TAB            */

	log_tab = lv_tabview_add_tab(tabview, "log");
	log_stream = lv_label_create(log_tab, NULL);

	logger::elog("gui: create log tab");

	/*            TASK            */

	if (!guiTask) {
		guiTask = std::make_shared<pros::Task>(gui_task, "gui task (xenon)");
		logger::elog("gui: create task (xenon)");
	} else {
		guiTask->resume();
		logger::elog("gui: resume task (xenon)");
	}

	started = true;
}

void
stop(void)
{
	lv_obj_clean(lv_scr_act());
	logger::elog("gui: clean screen");

	guiTask->suspend();
	logger::elog("gui: suspend task (xenon)");

	started = false;
}

} // namespace gui
