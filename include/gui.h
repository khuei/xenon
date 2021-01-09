#ifndef _GUI_H_
#define _GUI_H_

#include "pros/misc.hpp"
#include "port.h"

namespace gui {

extern int current_display;
extern bool started;

void switch_theme(pros::controller_digital_e_t, pros::controller_digital_e_t);
void switch_display(pros::controller_digital_e_t);
void switch_tab(pros::controller_digital_e_t, pros::controller_digital_e_t);
void toggle(pros::controller_digital_e_t);
void init(void);

} // namespace gui

#endif
