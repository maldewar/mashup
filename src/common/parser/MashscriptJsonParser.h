#ifndef MASHUP_COMMON_PARSER_MASHSCRIPTJSONPARSER_
#define MASHUP_COMMON_PARSER_MASHSCRIPTJSONPARSER_

#include <string>

#include "MashscriptParser.h"

/**
 * @brief Parser for JSON mashscript documents.
 *
 * The JSON parser only requires to implement the
 * pure virtual functions from MashscriptParser.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class MashscriptJsonParser : public MashscriptParser {
  public:
    /**
     * Class constructor.
     */
    MashscriptJsonParser();

  protected:
    /**
     * Reas a file containing mashscript code and populates
     * a boost::property_tree object accordingly.
     * @param file  Path to file.
     * @param pTree Property tree to populate.
     * @return Status or error code for the parsing.
     */
    virtual int ReadFormattedFile(std::string file,
      boost::property_tree::ptree& pTree);
    /**
     * Reads a string containing mashscript code and populates
     * a boost::property_tree object accordingly.
     * @param stream  String containing JSON formatted mashscript code.
     * @param pTree   Property tree to populate.
     * @return Status or error code for the parsing.
     */
    virtual int ReadFormattedStream(std::string stream,
      boost::property_tree::ptree& pTree);
};

#endif // MASHUP_COMMON_PARSER_MASHSCRIPTJSONPARSER_
