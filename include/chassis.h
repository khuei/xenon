#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#include "port.h"

namespace chassis {

void reset(void);
void set_brake(okapi::AbstractMotor::brakeMode);
void arcade(void);
void init(void);

} // namespace chassis

#endif
