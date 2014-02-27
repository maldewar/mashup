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

void Log::SetLevel(Level level) {
  if (! _is_init) {
    Init();
  }
  log4cxx::LoggerPtr root_logger = log4cxx::Logger::getRootLogger();
  switch(level) {
    case TRACE:
      root_logger->setLevel(log4cxx::Level::getTrace());
      break;
    case DEBUG:
      root_logger->setLevel(log4cxx::Level::getDebug());
      break;
    case INFO:
      root_logger->setLevel(log4cxx::Level::getInfo());
      break;
    case WARN:
      root_logger->setLevel(log4cxx::Level::getWarn());
      break;
    case ERROR:
      root_logger->setLevel(log4cxx::Level::getError());
      break;
    case FATAL:
      root_logger->setLevel(log4cxx::Level::getFatal());
      break;
  }
};

void Log::Init() {
  setlocale(LC_ALL, "");
  log4cxx::LoggerPtr rootlogger = log4cxx::Logger::getRootLogger();
  rootlogger->addAppender(new log4cxx::ConsoleAppender(new log4cxx::PatternLayout("[%-5p] %m%n")));
  _is_init = true;
};

log4cxx::LoggerPtr Log::GetLogger(const std::string logger_name) {
  if (! _is_init) {
    Init();
  }
  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(logger_name);
  return logger;
};
