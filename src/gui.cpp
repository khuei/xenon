#include <iostream>

#include "pros/rtos.hpp"

#include "gui.h"
#include "logger.h"
#include "port.h"

namespace gui {

lv_obj_t *tabview;

lv_obj_t *diagnostic_tab;
lv_obj_t *diagnostic_label;

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
		std::string text = "front left motor: " + std::to_string(front_left->getPosition()) + "\n" +
				   "front right motor: " + std::to_string(front_right->getPosition()) + "\n" +
				   "back left motor: " + std::to_string(back_left->getPosition()) + "\n" +
				   "back right motor: " + std::to_string(back_right->getPosition()) + "\n" +
				   "inertial sensor: " + std::to_string(imu->get()) + "\n";
		lv_label_set_text(diagnostic_label, text.c_str());

		std::string pos = "global_x: " + std::to_string(drive->getState().x.convert(okapi::inch)) + "_in\n" +
		                  "global_y: " + std::to_string(drive->getState().y.convert(okapi::inch)) + "_in\n" +
		                  "global_theta: " + std::to_string(drive->getState().theta.convert(okapi::degree)) + "_deg\n";
		lv_label_set_text(odom_label, pos.c_str());

		lv_label_set_text(log_stream, logger::ebuf());

		pros::delay(1000);
	}
}

void
init(void)
{
	tabview = lv_tabview_create(lv_scr_act(), NULL);

	diagnostic_tab = lv_tabview_add_tab(tabview, "Diagnostic");
	diagnostic_label = lv_label_create(diagnostic_tab, NULL);

	odom_tab = lv_tabview_add_tab(tabview, "Odometry");
	odom_label = lv_label_create(odom_tab, NULL);

	log_tab = lv_tabview_add_tab(tabview, "Log");
	log_stream = lv_label_create(log_tab, NULL);

	if (!guiTask)
		guiTask = std::make_shared<pros::Task>(gui_task, "gui task (xenon)");
	else
		guiTask->resume();

	started = true;
}

void
stop(void)
{
	lv_obj_clean(lv_scr_act());

	guiTask->suspend();

	started = false;
}

} // namespace gui
