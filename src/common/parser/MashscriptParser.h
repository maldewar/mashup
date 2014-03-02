#ifndef MASHUP_COMMON_PARSER_MASHSCRIPTPARSER_
#define MASHUP_COMMON_PARSER_MASHSCRIPTPARSER_

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "MashscriptParser.h"
#include "../../model/Mashup.h"

/**
 * Everything was OK with the parsing of data.
 */
const int PARSER_STATUS_OK = 0;

/**
 * Error related to the file containing the data.
 */
const int PARSER_ERROR_READING_FILE = -1;

/**
 * Error related to the syntaxis or semantic of
 * content of data.
 */
const int PARSER_ERROR_BAD_FORMAT = -2;

/**
 * Unknown error when parsing.
 */
const int PARSER_ERROR_UNKNOWN = -3;

/**
 * @brief Base class for parsing mashscript files or streams.
 *
 * The parser classes extending this base class should implement
 * only the reading of files or of streams providing a 
 * boost::property_tree object.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class MashscriptParser {
  public:
    /**
     * Class constructor.
     */
    MashscriptParser();
    /**
     * Reads a file containing mashscript code and populates
     * a Mashscript object accordingly.
     * @param file   Path to file.
     * @param mashup Mashup object to populate.
     * @return Status or error code for the parsing.
     */
    int FromFile(std::string file, Mashup& mashup);
    /**
     * Reads string containing mashscript code and populates
     * a Mashscript object accordingly.
     * @param stream String containing mashscript.
     * @param mashup Mashup object to populate.
     * @return Status or error code for the parsing.
     */
    int FromStream(std::string stream, Mashup& mashup);

  protected:
    /**
     * Reads a file containing mashscript code and populates
     * a boost::property_tree object accordingly.
     * @param file   Path to file.
     * @param p_tree Property tree to populate.
     * @return Status or error code for the parsing.
     */
    virtual int ReadFormattedFile(std::string file,
      boost::property_tree::ptree& p_tree) = 0;
    /**
     * Reads a string containing mashscript code and populates
     * a boost::property_tree object accordingly.
     * @param stream String containing JSON formatted mashscript data.
     * @param p_tree Property tree to populate.
     * @return Status or error code for the parsing.
     */
    virtual int ReadFormattedStream(std::string stream,
      boost::property_tree::ptree& p_tree) = 0;

  private:
    /**
     * Populates a mashup object taking values from a
     * boost::property_tree.
     * @param pTree  Property tree.
     * @param mashup Mashup object.
     * @return Status or error code for the parsing.
     */
    int ParseMashup(boost::property_tree::ptree& p_tree, Mashup& mashup);
};

#endif // MASHUP_COMMON_PARSER_MASHSCRIPTPARSER_
