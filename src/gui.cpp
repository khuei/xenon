#include "gui.h"
#include "config.h"
#include "debug.h"

namespace gui {

enum theme {
	LV_THEME_ALIEN,
	LV_THEME_MATERIAL,
	LV_THEME_MONO,
	LV_THEME_NEMO,
	LV_THEME_NIGHT,
	LV_THEME_ZEN,
};

int current_theme = 0;

enum display {
	GUI_CONFIG,
	GUI_DEBUG,
};

int current_display = 0;

bool started;

void
switch_theme(pros::controller_digital_e_t down, pros::controller_digital_e_t up)
{
	if (!gui::started)
		return;

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

	if (master.get_digital(down) || master.get_digital(up)) {
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
}

void
switch_display(pros::controller_digital_e_t toggle)
{
	if (master.get_digital_new_press(toggle)) {
		if (debug::started) {
			debug::stop();
			config::init();
			current_display = GUI_CONFIG;
		} else if (config::started) {
			config::stop();
			debug::init();
			current_display = GUI_DEBUG;
		}
	}
}

void
switch_tab(pros::controller_digital_e_t left, pros::controller_digital_e_t right)
{
	switch (current_display) {
	case GUI_CONFIG:
		config::switch_tab(left, right);
		break;
	case GUI_DEBUG:
		debug::switch_tab(left, right);
		break;
	}
}

void
toggle(pros::controller_digital_e_t toggle)
{
	if (master.get_digital_new_press(toggle)) {
		switch (current_display) {
		case GUI_CONFIG:
			if (!config::started)
				config::init();
			else
				config::stop();
			break;
		case GUI_DEBUG:
			if (!debug::started)
				debug::init();
			else
				debug::stop();
			break;
		}
	}
}

void
init(void)
{
	lv_theme_set_current(lv_theme_alien_init(205, NULL));
	switch (current_display) {
	case GUI_CONFIG:
		config::init();
		break;
	case GUI_DEBUG:
		debug::init();
		break;
	}

	started = true;
}

void
opcontrol(void)
{
	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
		gui::switch_display(pros::E_CONTROLLER_DIGITAL_UP);

	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
		gui::toggle(pros::E_CONTROLLER_DIGITAL_DOWN);

	if (gui::started) {
		gui::switch_tab(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);
		gui::switch_theme(pros::E_CONTROLLER_DIGITAL_DOWN, pros::E_CONTROLLER_DIGITAL_UP);
	}
}

} // namespace gui
