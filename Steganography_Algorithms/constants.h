#ifndef CONSTANTS
#define CONSTANTS

#include <string.h>

#define NUM_BITS_IN_CHAR 8

#ifndef __STDC_LIB_EXT1__
#define string_cpy(d, b, s) strcpy((d), (s))
#else 
#define string_cpy(d, b, s) strcpy_s((d), (b), (s))
#endif

#define M_PI 3.14159265358979323846

#endif