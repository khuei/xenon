#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "display/lvgl.h"
#include "port.h"

namespace debug {

extern bool started;

enum tab {
	CHASSIS_TAB,
	CHASSIS_GRAPH_TAB,
	INTAKE_TAB,
	INTAKE_GRAPH_TAB,
	ODOM_TAB,
	HEALTH_TAB,
	LOG_TAB,
};

extern int current_tab;

void switch_tab(pros::controller_digital_e_t, pros::controller_digital_e_t);

void init(void);
void stop(void);

} // namespace gui

#endif
