#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "display/lvgl.h"
#include "port.h"

namespace debug {

extern bool started;

void switch_tab(pros::controller_digital_e_t, pros::controller_digital_e_t);

void init(void);
void stop(void);

} // namespace gui

#endif
