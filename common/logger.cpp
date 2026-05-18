#include "logger.h"


void Logger::open(const char* serv) const {
    openlog(SERVER_ID, LOG_PID | LOG_NDELAY, LOG_USER);
    syslog(LOG_INFO, SERVER_INIT, serv);
}


void Logger::open(const char* host, const char* serv) const {
    openlog(CLIENT_ID, LOG_PID | LOG_NDELAY, LOG_USER);
    syslog(LOG_INFO, CLIENT_INIT, host, serv);
}


void Logger::info(const char* msg) const { syslog(LOG_INFO, MSG_FORMAT, msg); }


void Logger::err(const char* msg, const std::exception& e) const {
    syslog(LOG_ERR, EXCEPTION_FORMAT, msg, e.what());
}


void Logger::crit(const char* msg) const { syslog(LOG_CRIT, MSG_FORMAT, msg); }


Logger::~Logger() {
    info(END_MSG);
    closelog();
}
