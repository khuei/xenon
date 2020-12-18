#include <iostream>

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
	FILE *event_log = fopen("/usd/event.log", "a");
	fprintf(event_log, "%s\n", str);
	fclose(event_log);
	gui::reload_log();
}

} // namespace logger
