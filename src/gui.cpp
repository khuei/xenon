#include <iostream>

#include "pros/rtos.hpp"

#include "gui.h"
#include "health.h"
#include "logger.h"

namespace gui {

enum theme {
	LV_THEME_ALIEN,
	LV_THEME_MATERIAL,
	LV_THEME_MONO,
	LV_THEME_NEMO,
	LV_THEME_NIGHT,
	LV_THEME_ZEN,
};

enum tab {
	CHASSIS_TAB,
	CHASSIS_GRAPH_TAB,
	INTAKE_TAB,
	INTAKE_GRAPH_TAB,
	ODOM_TAB,
	HEALTH_TAB,
	LOG_TAB,
};

int current_theme = 0;
int current_tab = 0;

lv_obj_t *tabview;

lv_obj_t *chassis_tab;
lv_obj_t *chassis_left_speed_gauge;
lv_obj_t *chassis_right_speed_gauge;
lv_obj_t *chassis_left_label;
lv_obj_t *chassis_right_label;

lv_obj_t *chassis_graph_tab;
lv_obj_t *chassis_chart;
lv_obj_t *chassis_chart_label;
lv_chart_series_t *chassis_max_speed;
lv_chart_series_t *chassis_current_speed;

lv_obj_t *intake_tab;
lv_obj_t *intake_front_gauge;
lv_obj_t *intake_internal_gauge;
lv_obj_t *intake_front_label;
lv_obj_t *intake_internal_label;

lv_obj_t *intake_graph_tab;
lv_obj_t *intake_chart;
lv_obj_t *intake_chart_label;
lv_chart_series_t *intake_max_speed;
lv_chart_series_t *intake_front_current_speed;
lv_chart_series_t *intake_internal_current_speed;

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

		double chassis_left_speed =
			(fabs(front_left->getActualVelocity()) + fabs(back_left->getActualVelocity())) / 2;
		double chassis_right_speed =
			(fabs(front_right->getActualVelocity()) + fabs(back_right->getActualVelocity())) / 2;

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

		/*            CHASSIS GRAPH TAB            */

		lv_chart_set_next(chassis_chart, chassis_max_speed, 200);
		lv_chart_set_next(chassis_chart,
				  chassis_current_speed,
				  fabs((chassis_left_speed + chassis_right_speed) / 2));

		/*            INTAKE TAB            */

		double intake_front_speed =
			(fabs(left_intake->getActualVelocity()) + fabs(right_intake->getActualVelocity())) / 2;
		double intake_internal_speed =
			(fabs(top_intake->getActualVelocity()) + fabs(bottom_intake->getActualVelocity())) / 2;

		lv_gauge_set_value(intake_front_gauge, 0, fabs(intake_front_speed));
		lv_gauge_set_value(intake_internal_gauge, 0, fabs(intake_internal_speed));

		double intake_front_pos = (left_intake->getPosition() + right_intake->getPosition()) / 2;
		double intake_internal_pos = (top_intake->getPosition() + bottom_intake->getPosition()) / 2;

		std::string intake_front_str =
			std::to_string(intake_front_speed) + " RPM\n" + std::to_string(intake_front_pos) + " DEG\n";
		std::string intake_internal_str = std::to_string(intake_internal_speed) + " RPM\n" +
						  std::to_string(intake_internal_pos) + " DEG\n";

		lv_label_set_text(intake_front_label, intake_front_str.c_str());
		lv_label_set_text(intake_internal_label, intake_internal_str.c_str());

		/*            INTAKE GRAPH TAB            */

		lv_chart_set_next(intake_chart, intake_max_speed, 200);
		lv_chart_set_next(intake_chart, intake_front_current_speed, intake_front_speed);
		lv_chart_set_next(intake_chart, intake_internal_current_speed, intake_internal_speed);

		/*            ODOM TAB            */

		std::string pos = "global_x: " + std::to_string(drive->getState().x.convert(okapi::inch)) + "_in\n" +
				  "global_y: " + std::to_string(drive->getState().y.convert(okapi::inch)) + "_in\n" +
				  "global_theta: " + std::to_string(drive->getState().theta.convert(okapi::degree)) +
				  "_deg\n";
		lv_label_set_text(odom_label, pos.c_str());

		/*            HEALTH TAB            */

		std::string chassis_health_str = "Chassis\n\nfront left: " + checkhealth::motor(front_left) + "\n" +
						 "front right: " + checkhealth::motor(front_right) + "\n" +
						 "back left: " + checkhealth::motor(back_left) + "\n" +
						 "back right: " + checkhealth::motor(back_right);

		lv_label_set_text(chassis_health_label, chassis_health_str.c_str());

		std::string intake_health_str = "Intake\n\nleft: " + checkhealth::motor(left_intake) + "\n" +
						"right: " + checkhealth::motor(right_intake) + "\n" +
						"top: " + checkhealth::motor(top_intake) + "\n" +
						"bot: " + checkhealth::motor(bottom_intake);

		lv_label_set_text(intake_health_label, intake_health_str.c_str());

		std::string core_health_str = "Core\n\ncontroller: " + checkhealth::controller(master) + "\n" +
					      "battery: " + checkhealth::battery() + "\n" +
					      "usd: " + checkhealth::usd();

		lv_label_set_text(core_health_label, core_health_str.c_str());

		/*            LOG TAB            */

		lv_label_set_text(log_stream, logger::ebuf());

		pros::delay(100);
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
	case CHASSIS_GRAPH_TAB:
		lv_tabview_set_tab_act(tabview, CHASSIS_GRAPH_TAB, false);
		break;
	case INTAKE_TAB:
		lv_tabview_set_tab_act(tabview, INTAKE_TAB, false);
		break;
	case INTAKE_GRAPH_TAB:
		lv_tabview_set_tab_act(tabview, INTAKE_GRAPH_TAB, false);
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
switch_theme(pros::controller_digital_e_t down, pros::controller_digital_e_t up)
{
	if (master.get_digital_new_press(down)) {
		if (current_theme > LV_THEME_ALIEN)
			current_theme--;
		else
			current_theme = LV_THEME_ZEN;
	}

	if (master.get_digital_new_press(up)) {
		if (current_theme < LV_THEME_ZEN)
			current_theme++;
		else
			current_theme = LV_THEME_ALIEN;
	}

	switch (current_theme) {
	case LV_THEME_ALIEN:
		lv_theme_set_current(lv_theme_alien_init(205, NULL));
		break;
	case LV_THEME_MATERIAL:
		lv_theme_set_current(lv_theme_material_init(205, NULL));
		break;
	case LV_THEME_MONO:
		lv_theme_set_current(lv_theme_mono_init(205, NULL));
		break;
	case LV_THEME_NEMO:
		lv_theme_set_current(lv_theme_nemo_init(205, NULL));
		break;
	case LV_THEME_NIGHT:
		lv_theme_set_current(lv_theme_night_init(205, NULL));
		break;
	case LV_THEME_ZEN:
		lv_theme_set_current(lv_theme_zen_init(205, NULL));
		break;
	}
}

void
init(void)
{
	/*            MAIN TAB            */

	tabview = lv_tabview_create(lv_scr_act(), NULL);

	lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_INDIC, &lv_style_transp);

	logger::elog("gui: create main tab");

	/*            CHASSIS TAB            */

	chassis_tab = lv_tabview_add_tab(tabview, "chassis");
	lv_page_set_sb_mode(chassis_tab, LV_SB_MODE_OFF);

	chassis_left_speed_gauge = lv_gauge_create(chassis_tab, NULL);
	lv_gauge_set_range(chassis_left_speed_gauge, 0, 200);
	lv_obj_set_size(chassis_left_speed_gauge, 180, 150);
	lv_obj_align(chassis_left_speed_gauge, NULL, LV_ALIGN_IN_LEFT_MID, 20, 0);
	chassis_left_label = lv_label_create(chassis_tab, NULL);
	lv_obj_align(chassis_left_label, NULL, LV_ALIGN_CENTER, -150, 50);

	chassis_right_speed_gauge = lv_gauge_create(chassis_tab, NULL);
	lv_gauge_set_range(chassis_right_speed_gauge, 0, 200);
	lv_obj_set_size(chassis_right_speed_gauge, 180, 150);
	lv_obj_align(chassis_right_speed_gauge, NULL, LV_ALIGN_IN_RIGHT_MID, -20, 0);
	chassis_right_label = lv_label_create(chassis_tab, NULL);
	lv_obj_align(chassis_right_label, NULL, LV_ALIGN_CENTER, 90, 50);

	logger::elog("gui: create chassis tab");

	/*            CHASSIS GRAPH TAB            */

	chassis_graph_tab = lv_tabview_add_tab(tabview, "graph");
	lv_page_set_sb_mode(chassis_graph_tab, LV_SB_MODE_OFF);

	chassis_chart = lv_chart_create(chassis_graph_tab, NULL);
	lv_chart_set_type(chassis_chart, LV_CHART_TYPE_LINE);
	lv_chart_set_range(chassis_chart, 0, 250);
	lv_chart_set_series_width(chassis_chart, 2);
	lv_chart_set_div_line_count(chassis_chart, 10, 20);
	lv_obj_align(chassis_chart, NULL, LV_ALIGN_OUT_LEFT_TOP, 0, 0);
	lv_obj_set_size(chassis_chart, 700, 140);

	chassis_max_speed = lv_chart_add_series(chassis_chart, LV_COLOR_RED);
	chassis_current_speed = lv_chart_add_series(chassis_chart, LV_COLOR_LIME);

	chassis_chart_label = lv_label_create(chassis_chart, NULL);
	lv_label_set_recolor(chassis_chart_label, true);
	lv_label_set_text(chassis_chart_label,
			  "#ff0000 max_speed#\n"
			  "#00ff00 current_speed#\n");
	lv_obj_align(chassis_chart_label, NULL, LV_ALIGN_CENTER, -120, 50);

	logger::elog("gui: create chassis graph");

	/*            INTAKE TAB            */

	intake_tab = lv_tabview_add_tab(tabview, "intake");
	lv_page_set_sb_mode(intake_tab, LV_SB_MODE_OFF);

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

	/*            INTAKE GRAPH TAB            */

	intake_graph_tab = lv_tabview_add_tab(tabview, "graph");
	lv_page_set_sb_mode(intake_graph_tab, LV_SB_MODE_OFF);

	intake_chart = lv_chart_create(intake_graph_tab, NULL);
	lv_chart_set_type(intake_chart, LV_CHART_TYPE_LINE);
	lv_chart_set_range(intake_chart, 0, 250);
	lv_chart_set_series_width(intake_chart, 2);
	lv_chart_set_div_line_count(intake_chart, 10, 20);
	lv_obj_align(intake_chart, NULL, LV_ALIGN_OUT_LEFT_TOP, 0, 0);
	lv_obj_set_size(intake_chart, 700, 140);

	intake_max_speed = lv_chart_add_series(intake_chart, LV_COLOR_RED);
	intake_front_current_speed = lv_chart_add_series(intake_chart, LV_COLOR_LIME);
	intake_internal_current_speed = lv_chart_add_series(intake_chart, LV_COLOR_CYAN);

	intake_chart_label = lv_label_create(intake_chart, NULL);
	lv_label_set_recolor(intake_chart_label, true);
	lv_label_set_text(intake_chart_label,
			  "#ff0000 max_speed#\n"
			  "#00ff00 front_speed#\n"
			  "#00ffff internal_speed\n");
	lv_obj_align(intake_chart_label, NULL, LV_ALIGN_CENTER, -90, 40);

	logger::elog("gui: create intake graph");

	/*            ODOM TAB            */

	odom_tab = lv_tabview_add_tab(tabview, "odom");
	odom_label = lv_label_create(odom_tab, NULL);

	logger::elog("gui: create odom tab");

	/*            HEALTH TAB            */

	health_tab = lv_tabview_add_tab(tabview, "health");
	lv_page_set_sb_mode(health_tab, LV_SB_MODE_OFF);

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
	lv_page_set_sb_mode(log_tab, LV_SB_MODE_OFF);

	logger::elog("gui: create log tab");
	lv_label_set_long_mode(log_stream, LV_LABEL_LONG_SCROLL);

	/*            TASK            */

	if (!guiTask) {
		guiTask = std::make_shared<pros::Task>(gui_task, "gui task (xenon)");
		guiTask->set_priority(TASK_PRIORITY_MIN);
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
