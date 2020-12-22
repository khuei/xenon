#include <iostream>

#include "pros/rtos.hpp"

#include "logger.h"
#include "gui.h"
#include "port.h"

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
}

void
init(void)
{
	fclose(fopen("/usd/event.log", "w"));
	logger::elog("logger: clear /usd/event.log");

	okapi::Logger::setDefaultLogger(okapi_logger);
	logger::elog("logger: set okapi_logger as default logger");
}

} // namespace logger
