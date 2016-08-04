#ifndef DEFINES_H
#define DEFINES_H

#include <math.h>

#ifndef M_PI
# define M_PI		3.14159265358979323846
#endif
#ifndef RADIAN
# define RADIAN (M_PI / 180)
#endif

#define D2R(x) (x*RADIAN)
#define R2D(x) (x/RADIAN)

#define EARTH_RADIUS 6371.0

#endif // DEFINES_H
