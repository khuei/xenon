#include <iostream>

#include "pros/rtos.hpp"
#include "logger.h"
#include "gui.h"

namespace logger {

char *
ebuf(void)
{
	FILE *stream;
	double length;
	char *buffer;

	stream = fopen("/usd/event.log", "r");

	if (stream) {
		fseek(stream, 0L, SEEK_END);
		length = ftell(stream);
		rewind(stream);
		buffer = (char *)malloc(length);

		if (buffer)
			fread(buffer, 1, length, stream);

		fclose(stream);
	}

	return buffer;
}

void
elog(const char *str)
{

	std::uint32_t millisecond = pros::millis();
	std::uint32_t second = millisecond / 1000;
	std::uint32_t minute = second / 60;
	std::uint32_t hour = minute / 60;

	FILE *event_log = fopen("/usd/event.log", "a");
	fprintf(event_log, "[%02ld:%02ld:%02ld] %s\n", hour, minute, second, str);
	fclose(event_log);
	gui::reload_log();
}

} // namespace logger
