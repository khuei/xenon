#ifndef GUI
#define GUI

#include "display/lvgl.h"
#include "port.h"

namespace gui {

extern bool started;
void switch_tab(pros::controller_digital_e_t, pros::controller_digital_e_t);
void switch_theme(pros::controller_digital_e_t, pros::controller_digital_e_t);
void init(void);
void stop(void);

} // namespace gui

#endif
