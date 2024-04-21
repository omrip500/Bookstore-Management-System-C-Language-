#ifndef MACROS_H_
#define MACROS_H_

#include "Schedule.h"

#define MIN(x, y)	(x) < (y) ? (1) : (0)

#define SUM(x, y)	(x)+(y)

#define EQUALS(x, y) (x) == (y) ? (1) : (0)

#define ARRLOCATION(x)	(x) - OPENING_HOUR

#define CHECK_RETURN_NULL(ptr) {if(!ptr) return 0;}

#endif
