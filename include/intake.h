#ifndef INTAKE
#define INTAKE

#include "port.h"

namespace intake {

void tuner(pros::controller_digital_e_t, pros::controller_digital_e_t);
void reset(void);
void set_brake(okapi::AbstractMotor::brakeMode);
void run_front(double);
void run_internal(double);
void run_both(double);
void init(void);

} // namespace intake

#endif
