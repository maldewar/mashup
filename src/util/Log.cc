#include "Log.h"

#include <iostream>

#include <log4cxx/level.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>

bool Log::_is_init = false;

void Log::Trace(const std::string msg, const std::string logger_name) {
  LOG4CXX_TRACE(GetLogger(logger_name), msg);
};

void Log::Debug(const std::string msg, const std::string logger_name) {
  LOG4CXX_DEBUG(GetLogger(logger_name), msg);
};

void Log::Info(const std::string msg, const std::string logger_name) {
  LOG4CXX_INFO(GetLogger(logger_name), msg);
};

void Log::Warn(const std::string msg, const std::string logger_name) {
  LOG4CXX_WARN(GetLogger(logger_name), msg);
};

void Log::Error(const std::string msg, const std::string logger_name) {
  LOG4CXX_ERROR(GetLogger(logger_name), msg);
};

void Log::Fatal(const std::string msg, const std::string logger_name) {
  LOG4CXX_FATAL(GetLogger(logger_name), msg);
};

log4cxx::LoggerPtr Log::GetLogger(const std::string logger_name) {
  if (! _is_init) {
    log4cxx::LoggerPtr rootlogger = log4cxx::Logger::getRootLogger();
    rootlogger->addAppender(new log4cxx::ConsoleAppender(new log4cxx::PatternLayout("[%-5p] %m%n")));
    _is_init = true;
  }
  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(logger_name);
  return logger;
};
