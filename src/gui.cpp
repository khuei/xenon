#include "gui.h"

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

} // namespace gui
