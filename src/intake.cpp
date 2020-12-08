#include "intake.h"

namespace intake {

void run_front(double speed) {
	left_intake.moveVelocity(speed);
	right_intake.moveVelocity(speed);
}

void run_internal(double speed) {
	top_intake.moveVelocity(speed);
	bottom_intake.moveVelocity(speed);
}

void run_both(double speed) {
	left_intake.moveVelocity(speed);
	right_intake.moveVelocity(speed);
	top_intake.moveVelocity(speed);
	bottom_intake.moveVelocity(speed);
}

} // namespace intake
