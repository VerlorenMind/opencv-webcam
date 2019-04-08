#ifndef OPENCV_LAB_LOGGERINIT_H
#define OPENCV_LAB_LOGGERINIT_H

#define LOG_FILE "log.txt"
#define LOG_INIT
#include "Logger.h"
Logger logger (LOG_FILE);
#undef LOG_INIT
#endif //OPENCV_LAB_LOGGERINIT_H
