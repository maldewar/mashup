#ifndef MASHUP_COMMON_PARSER_MASHSCRIPTPARSER_
#define MASHUP_COMMON_PARSER_MASHSCRIPTPARSER_

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "MashscriptParser.h"
#include "../../model/Mashup.h"

const int PARSER_STATUS_OK            = 0;
const int PARSER_ERROR_FILE_NOT_FOUND = -1;
const int PARSER_ERROR_BAD_FORMATT    = -2;

/**
 * @file
 * @brief Base class for parsing mashscript files or streams.
 *
 * The parser classes extending this base class should implement
 * only the reading of files or of streams providing a boost::property_tree
 * object.
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
     * Reads a file containing mashupscript code and popules
     * a boost::property_tree object accordingly.
     * @param file  Path to file.
     * @param pTree Property tree to populate.
     * @return Status or error code for the parsing.
     */
    virtual int ReadFormattedFile(std::string file,
      boost::property_tree::ptree& pTree) = 0;
    /**
     * Reads a string containing mashupscript code and popules
     * a boost::property_tree object accordingly.
     * @param file  Path to file.
     * @param pTree Property tree to populate.
     * @return Status or error code for the parsing.
     */
    virtual int ReadFormattedStream(std::string stream,
      boost::property_tree::ptree& pTree) = 0;

  private:
    /**
     * Populates a mashup object taking values from a
     * boost::property_tree.
     * @param pTree  Property tree.
     * @param mashup Mashup object.
     * @return Status or error code for the parsing.
     */
    int ParseMashup(boost::property_tree::ptree& pTree, Mashup& mashup);
};

#endif // MASHUP_COMMON_PARSER_MASHSCRIPTPARSER_
