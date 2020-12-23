#include <iostream>

#include "pros/rtos.hpp"

#include "gui.h"
#include "logger.h"
#include "port.h"

namespace gui {

lv_obj_t *tabview;

lv_obj_t *diagnostic_tab;
lv_obj_t *diagnostic_label;

lv_obj_t *log_tab;
lv_obj_t *log_stream;

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

	log_tab = lv_tabview_add_tab(tabview, "Log");
	log_stream = lv_label_create(log_tab, NULL);

	pros::Task guiTask(gui_task);
}

} // namespace gui
