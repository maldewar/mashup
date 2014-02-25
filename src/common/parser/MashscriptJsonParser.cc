#include "MashscriptJsonParser.h"

#include <iostream>

#include <boost/property_tree/json_parser.hpp>

#include "../../util/Log.h"

namespace pt = boost::property_tree;

MashscriptJsonParser::MashscriptJsonParser() : MashscriptParser() {
};

int MashscriptJsonParser::ReadFormattedFile(std::string file,
  boost::property_tree::ptree& p_tree) {
  try {
    pt::read_json(file, p_tree);
    LOG_TRACE("Mashscript file read: " << file, LOGGER_PARSER);
  } catch (pt::json_parser_error& err) {
    LOG_ERROR(err.what(), LOGGER_PARSER);
    return PARSER_ERROR_READING_FILE;
  } catch (...) {
    LOG_ERROR("Unknown error while trying to access json file " << file, LOGGER_PARSER);
    return PARSER_ERROR_UNKNOWN;
  }
  return PARSER_STATUS_OK;
};

int MashscriptJsonParser::ReadFormattedStream(std::string stream,
  boost::property_tree::ptree& p_tree) {
  return PARSER_STATUS_OK;
};
