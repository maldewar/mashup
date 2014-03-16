#include "PathBuilder.h"

#include "../../util/Log.h"

PathBuilder::PathBuilder() {};

std::string PathBuilder::GetAbsolute(const std::string& path) {
  std::string path_result;
  fs::path fs_absolute_path = fs::system_complete(path);
  path_result = GetAbsoluteFromPath(fs_absolute_path);
  //delete fs_absolute_path;
  return path_result;
};

std::string PathBuilder::GetAbsolute(const std::string& path,
                                     const std::string& base) {
  LOG_TRACE("PATH BUILDER path: " << path, LOGGER_BUILDER);
  LOG_TRACE("PATH BUILDER base: " << base, LOGGER_BUILDER);
  std::string path_result;
  fs::path fs_path(path); 
  fs::path fs_absolute_path = fs::system_complete(operator/(base, fs_path));
  LOG_TRACE("PATH BUILDER composed: " << fs_absolute_path, LOGGER_BUILDER);
  path_result = GetAbsoluteFromPath(fs_absolute_path);
  //delete fs_absolute_path;
  return path_result;
};

std::string PathBuilder::GetAbsoluteFromPath(const boost::filesystem::path& path) {
  try {
    if (path.is_complete() && fs::exists(path))
      return (fs::canonical(path)).native();
  } catch (...) {}
  return "";
};
