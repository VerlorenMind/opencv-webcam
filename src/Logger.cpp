#include "Logger.h"

Logger::Logger(const char *logfile) {
    log = fopen (logfile, "w");
}


Logger::~Logger () {
    write ("End of log");
    fclose (log);
};

void Logger::write(const char *format, ...) {
    va_list args;
    va_start (args, format);
    vfprintf (log, format, args);
    fprintf (log, "\n");
    va_end (args);
}