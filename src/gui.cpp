#include <iostream>

#include "gui.h"
#include "logger.h"
#include "port.h"

namespace gui {

lv_obj_t *tabview;

lv_obj_t *diagnostic_tab;
lv_obj_t *diagnostic_label;

lv_obj_t *log_tab;
lv_obj_t *log_stream;

void
reload_diagnostic(void)
{
	std::string text =
	"front left motor: " + std::to_string(front_left.getPosition()) + "\n" +
	"front right motor: " + std::to_string(front_right.getPosition()) + "\n" +
	"back left motor: " + std::to_string(back_left.getPosition()) + "\n" +
	"back right motor: " + std::to_string(back_right.getPosition()) + "\n" +
	"inertial sensor: " + std::to_string(imu.get()) + "\n";
	lv_label_set_text(diagnostic_label, text.c_str());
}

void
reload_log(void)
{
	lv_label_set_text(log_stream, logger::ebuf());
}

void
init(void)
{
	tabview = lv_tabview_create(lv_scr_act(), NULL);

	diagnostic_tab = lv_tabview_add_tab(tabview, "Diagnostic");
	diagnostic_label = lv_label_create(diagnostic_tab, NULL);
	reload_diagnostic();

	log_tab = lv_tabview_add_tab(tabview, "Log");
	log_stream = lv_label_create(log_tab, NULL);
	lv_label_set_text(log_stream, logger::ebuf());
}

} // namespace gui
