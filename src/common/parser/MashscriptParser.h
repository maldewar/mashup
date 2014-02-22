#ifndef MASHUP_COMMON_PARSER_MASHSCRIPTPARSER_
#define MASHUP_COMMON_PARSER_MASHSCRIPTPARSER_

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "MashscriptParser.h"
#include "../../model/Mashup.h"

const int PARSER_STATUS_OK            = 0;
const int PARSER_ERROR_FILE_NOT_FOUND = -1;
const int PARSER_ERROR_BAD_FORMATT    = -2;

class MashscriptParser {
  public:
    MashscriptParser();
    int FromFile(std::string file, Mashup& mashup);
    int FromStream(std::string stream, Mashup& mashup);

  protected:
    virtual int ReadFormattedFile(std::string file,
      boost::property_tree::ptree& pTree) = 0;
    virtual int ReadFormattedStream(std::string stream,
      boost::property_tree::ptree& pTree) = 0;

  private:
    int ParseMashup(boost::property_tree::ptree& pTree, Mashup& mashup);
};

#endif // MASHUP_COMMON_PARSER_MASHSCRIPTPARSER_
