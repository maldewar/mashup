#include "MashupBuilder.h"

#include "../../util/Log.h"

namespace pt = boost::property_tree;

MashupBuilder::MashupBuilder() {
};

int MashupBuilder::Build(Mashup& mashup, const boost::property_tree::ptree& p_tree) {
  mashup.version = p_tree.get<std::string>("version","");
  LOG_TRACE("Version " << mashup.version, LOGGER_BUILDER);
  return BUILDER_STATUS_OK;
};
