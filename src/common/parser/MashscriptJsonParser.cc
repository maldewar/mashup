#include "MashscriptJsonParser.h"

#include <iostream>

#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

MashscriptJsonParser::MashscriptJsonParser() : MashscriptParser() {
};

int MashscriptJsonParser::ReadFormattedFile(std::string file,
  boost::property_tree::ptree& pTree) {
  pt::read_json(file, pTree);
  return PARSER_STATUS_OK;
};

int MashscriptJsonParser::ReadFormattedStream(std::string stream,
  boost::property_tree::ptree& pTree) {
  return PARSER_STATUS_OK;
};
