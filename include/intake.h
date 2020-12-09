#ifndef INTAKE
#define INTAKE

#include "port.h"

namespace intake {

void reset(void);
void set_brake(okapi::AbstractMotor::brakeMode);
void run_front(double);
void run_internal(double);
void run_both(double);

} // namespace intake

#endif
