#ifndef UTIL_H
#define UTIL_H

#define AS_BLOCK(decl, obj) \
	decl objAs = dynamic_cast<decl>(obj);\
	if (objAs != NULL)

#endif