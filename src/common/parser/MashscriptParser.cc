#include "MashscriptParser.h"

#include <iostream>

#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;

MashscriptParser::MashscriptParser() {
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

int MashscriptParser::ParseMashup(boost::property_tree::ptree& pTree, Mashup& mashup) {
  std::cout << "Mashup title: " << pTree.get<std::string>("mashup.title") <<".\n";
  return PARSER_STATUS_OK;
};
