#ifndef LOGGER
#define LOGGER

namespace logger {

const char *logfile(void);
char *ebuf(void);
void elog(const char *);
void init(void);

} // namespace logger

#endif
