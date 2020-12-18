#include <iostream>

#include "gui.h"
#include "logger.h"

namespace gui {

lv_obj_t *tabview;
lv_obj_t *log_tab;
lv_obj_t *log_stream;

void
reload_log(void) {
	lv_label_set_text(log_stream, logger::ebuf());
}

void
init(void)
{
	tabview = lv_tabview_create(lv_scr_act(), NULL);
	log_tab = lv_tabview_add_tab(tabview, "Log");
	log_stream = lv_label_create(log_tab, NULL);
	lv_label_set_text(log_stream, logger::ebuf());
}

} // namespace gui
