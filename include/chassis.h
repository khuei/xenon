#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#include "port.h"

namespace chassis {

void move(okapi::QLength, okapi::QLength, okapi::QAngle);
void arcade(void);
void init(void);

} // namespace chassis

#endif
