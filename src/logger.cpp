#include <iostream>

#include "pros/rtos.hpp"

#include "logger.h"
#include "port.h"

namespace logger {

const char *
logfile(void)
{
	const char *name;
	if (pros::usd::is_installed())
		name = "/usd/event.log";
	else
		name = "/ser/sout";

	return name;
}

char *
ebuf(void)
{
	FILE *stream;
	double length;
	char *buffer;

	stream = fopen(logfile(), "r");

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

	std::uint32_t hour = millisecond / 3600000;
	millisecond -= hour * 3600000;

	std::uint32_t minute = millisecond / 60000;
	millisecond -= minute * 60000;

	std::uint32_t second = millisecond / 1000;
	millisecond -= second * 1000;

	FILE *event_log = fopen(logfile(), "a");
	fprintf(event_log,
		"[%02ld:%02ld:%02ld] %s\n",
		hour,
		minute,
		second,
		str);
	fclose(event_log);
}

void
init(void)
{
	fclose(fopen(logfile(), "w"));
	logger::elog("logger: clear log buffer");

	okapi::Logger::setDefaultLogger(okapi_logger);
	logger::elog("logger: set okapi_logger as default logger");
}

} // namespace logger
