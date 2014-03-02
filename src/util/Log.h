#ifndef MASHUP_UTIL_LOG_
#define MASHUP_UTIL_LOG_

#include <string>
#include <sstream>

#include <log4cxx/logger.h>

/**
 * Logger name for parsing streams or file content.
 */
const std::string LOGGER_PARSER  = "common.parser";

/**
 * Logger name for building objects.
 */
const std::string LOGGER_BUILDER = "common.builder";

/**
 * Macro for logging trace messages.
 */
#define LOG_TRACE(msg, logger_name) { \
  std::ostringstream oss; \
  oss << msg; \
  Log::Trace(oss.str(), logger_name); }

/**
 * Macro for logging debug messages.
 */
#define LOG_DEBUG(msg, logger_name) { \
  std::ostringstream oss; \
  oss << msg; \
  Log::Debug(oss.str(), logger_name); }

/**
 * Macro for logging info messages.
 */
#define LOG_INFO(msg, logger_name) { \
  std::ostringstream oss; \
  oss << msg; \
  Log::Info(oss.str(), logger_name); }

/**
 * Macro for logging warning messages.
 */
#define LOG_WARN(msg, logger_name) { \
  std::ostringstream oss; \
  oss << msg; \
  Log::Warn(oss.str(), logger_name); }

/**
 * Macro for logging error messages.
 */
#define LOG_ERROR(msg, logger_name) { \
  std::ostringstream oss; \
  oss << msg; \
  Log::Error(oss.str(), logger_name); }

/**
 * Macro for logging fatal messages.
 */
#define LOG_FATAL(msg, logger_name) { \
  std::ostringstream oss; \
  oss << msg; \
  Log::Fatal(oss.str(), logger_name); }

/**
 * @brief Logging class.
 *
 * This class is intended to work as a wrapper class for
 * Log4Cxx. http://logging.apache.org/log4cxx/
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class Log {
  public:
    /**
     * Log level.
     */
    enum Level {TRACE, DEBUG, INFO, WARN, ERROR, FATAL};

  public:
    /**
     * Log function for trace messages. Lowest level for logging.
     * Used mainly on development environment to inspect program
     * flow.
     * @param msg         Message to log.
     * @param logger_name Name of the logger.
     */
    static void Trace(const std::string msg, const std::string logger_name);
    /**
     * Log function for debug messages. Useful for debuging and testing.
     * @param msg         Message to log.
     * @param logger_name Name of the logger.
     */
    static void Debug(const std::string msg, const std::string logger_name);
    /**
     * Log function for information messages. Used for logging useful
     * information about the normal flow of the application.
     * @param msg         Message to log.
     * @param logger_name Name of the logger.
     */
    static void Info(const std::string msg, const std::string logger_name);
    /**
     * Log function for warning messages. Used for informing about abnormal
     * states the application itself try to solve so it does not generate
     * an error later.
     * @param msg         Message to log.
     * @param logger_name Name of the logger.
     */
    static void Warn(const std::string msg, const std::string logger_name);
    /**
     * Log function for error messages. Error messages compromise processes
     * but the application is still able to run.
     * @param msg         Message to log.
     * @param logger_name Name of the logger.
     */
    static void Error(const std::string msg, const std::string logger_name);
    /**
     * Log function for fatal messages. Fatal messages are total blockers for
     * for the application, not able to run on these situations.
     * @param msg         Message to log.
     * @param logger_name Name of the logger.
     */
    static void Fatal(const std::string msg, const std::string logger_name);
    /**
     * Sets the global level for logging.
     * @param level Logging level.
     */
    static void SetLevel(Level level);

  private:
    /**
     * Private constructor for static class.
     */
    Log();
    /**
     * Initializes Log4Cxx.
     */
    static void Init();
    /**
     * Get a LoggerPtr given the name of a logger.
     * @param logger_name Logger name.
     * @return LoggerPtr to be used with Log4Cxx macros.
     */
    static log4cxx::LoggerPtr GetLogger(const std::string logger_name);

  private:
    /**
     * Controls static initialization for Log4Cxx.
     */
    static bool _is_init;

};

#endif // MASHUP_UTIL_LOG_
