#ifndef _INTAKE_H_
#define _INTAKE_H_

#include "port.h"

namespace intake {

void reset(void);
void set_brake(okapi::AbstractMotor::brakeMode);
void run_front(double);
void run_internal(double);
void run_both(double);
void init(void);
void opcontrol(void);

} // namespace intake

#endif
