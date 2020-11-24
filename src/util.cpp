#include <iostream>
#include <cstdlib>
#include <cstdarg>

void
die(const char *errstr, ...)
{
	va_list ap;

	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);

	exit(1);
}

void
switcher(int *num, int total)
{
	if (*num < total)
		++(*num);
	else
		*num = 0;
}
