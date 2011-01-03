#ifndef UTIL_H
#define UTIL_H

#define AS_BLOCK(type, var, obj) \
	type var = dynamic_cast<type>(obj);\
	if (var != NULL)

#endif
