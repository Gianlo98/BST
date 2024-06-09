#include "logger.h"

void Logger::connect(LoggerOutput* p_rl){
    this->rl = p_rl;
    log("Logger connected");
}

void Logger::log(QString qs){
    rl->logAppend(qs);
}
