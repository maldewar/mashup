#ifndef MASHUP_COMMON_BUILDER_PATHBUILDER_
#define MASHUP_COMMON_BUILDER_PATHBUILDER_

#include <string>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

/**
 * @brief Utility to build paths.
 *
 * The resulting paths are absolute.
 * @author Waldemar Sánchez
 * @date March, 2014
 */
class PathBuilder {
  public:
    /**
     * Composes a complete path given a location string.
     * @param path Location string.
     * @return Absolute path, if invalid then empty string
     *         is returned.
     */
    static std::string GetAbsolute(const std::string& path);
    /**
     * Composes a complete path given a location string and
     * a base location to parent it.
     * @param path Location string.
     * @param base Base location string.
     * @return Absolute path, if invalid then empty string
     *         is returned.
     */
    static std::string GetAbsolute(const std::string& path,
                                   const std::string& base);

  private:
    /**
     * Private constructor.
     */
    PathBuilder();
    /**
     * Returns an absolute path if this is complete.
     * @param path Boost path object.
     * @return Absolite path, if invalid then emptry string
     *         is returned.
     */
    static std::string GetAbsoluteFromPath(const boost::filesystem::path& path);
};

#endif // MASHUP_COMMON_BUILDER_PATHBUILDER_
