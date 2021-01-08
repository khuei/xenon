#ifndef _LOGGER_H_
#define _LOGGER_H_

namespace logger {

const char *logfile(void);
char *ebuf(void);
void elog(const char *);
void init(void);

} // namespace logger

#endif
