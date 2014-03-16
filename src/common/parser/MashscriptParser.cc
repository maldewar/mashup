#include "MashscriptParser.h"

#include <iostream>

#include <boost/property_tree/ptree.hpp>

#include "../builder/MashupBuilder.h"

namespace pt = boost::property_tree;

MashscriptParser::MashscriptParser() {
  assets_path = "";
};

int MashscriptParser::FromFile(const std::string file, Mashup& mashup) {
  int result;
  pt::ptree pTree;
  result = ReadFormattedFile(file, pTree);
  if (result < PARSER_STATUS_OK)
    return result;
  result = ParseMashup(pTree, mashup);
  if (result < PARSER_STATUS_OK)
    return result;
  return PARSER_STATUS_OK;
};

int MashscriptParser::FromStream(const std::string stream, Mashup& mashup) {
  return PARSER_STATUS_OK;
};

int MashscriptParser::ParseMashup(boost::property_tree::ptree& p_tree, Mashup& mashup) {
  int result;
  MashupBuilder* builder = new MashupBuilder();
  builder->assets_path = assets_path;
  result = builder->Build(mashup, p_tree);
  if (result < BUILDER_STATUS_OK)
    result = PARSER_ERROR_BAD_FORMAT;
  delete builder;
  return result;
};
