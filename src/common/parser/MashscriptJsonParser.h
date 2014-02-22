#ifndef MASHUP_COMMON_PARSER_MASHSCRIPTJSONPARSER_
#define MASHUP_COMMON_PARSER_MASHSCRIPTJSONPARSER_

#include <string>

#include "MashscriptParser.h"

class MashscriptJsonParser : public MashscriptParser {
  public:
    MashscriptJsonParser();

  protected:
    virtual int ReadFormattedFile(std::string file,
      boost::property_tree::ptree& pTree);
    virtual int ReadFormattedStream(std::string stream,
      boost::property_tree::ptree& pTree);
};

#endif // MASHUP_COMMON_PARSER_MASHSCRIPTJSONPARSER_
