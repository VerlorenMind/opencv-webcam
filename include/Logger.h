#ifndef OPENCV_LAB_LOGGER_H
#define OPENCV_LAB_LOGGER_H

#include <cstdio>
#include <cstdarg>

class Logger {
private:
    FILE *log;
public:
    Logger(const char *logfile);
    ~Logger();

    void write(const char *format, ...);

};

#ifndef LOG_INIT
extern Logger logger;
#endif

#endif //OPENCV_LAB_LOGGER_H
