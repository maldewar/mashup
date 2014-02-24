#include "Log.h"

#include <iostream>

#include <log4cxx/level.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>

log4cxx::LoggerPtr Log::_logger = log4cxx::Logger::getLogger("Mashup");

void Log::Trace(const std::string msg) {
};

void Log::Debug(const std::string msg) {
};

void Log::Info(const std::string msg) {
};

void Log::Warn(const std::string msg) {
};

void Log::Error(const std::string msg) {
};

void Log::Fatal(const std::string msg) {
};

void Log::Message(const std::string msg, int error_level) {
  std::cout << msg;
};
