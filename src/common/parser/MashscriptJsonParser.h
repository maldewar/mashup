#ifndef MASHUP_COMMON_PARSER_MASHSCRIPTJSONPARSER_
#define MASHUP_COMMON_PARSER_MASHSCRIPTJSONPARSER_

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class MashscriptJsonParser {
  public:
    MashscriptJsonParser();
    ~MashscriptJsonParser();
    std::string ParseScript(std::string script);
};

#endif // MASHUP_COMMON_PARSER_MASHSCRIPTJSONPARSER_
