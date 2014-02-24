#ifndef MASHUP_UTIL_LOG_
#define MASHUP_UTIL_LOG_

#include <string>

#include <log4cxx/logger.h>

class Log {
  public:
    static void Trace(const std::string msg);
    static void Debug(const std::string msg);
    static void Info(const std::string msg);
    static void Warn(const std::string msg);
    static void Error(const std::string msg);
    static void Fatal(const std::string msg);

  private:
    /**
     * Class constructor.
     */
    Log();
    static void Message(const std::string msg, int error_level);

  private:
    static log4cxx::LoggerPtr _logger;
    
};

#endif // MASHUP_UTIL_LOG_
