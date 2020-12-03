#ifndef CHASSIS
#define CHASSIS

#include "port.h"

namespace chassis {

void reset(void);
void set_brake(okapi::AbstractMotor::brakeMode);
void arcade(void);

} // namespace chassis

#endif
