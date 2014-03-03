#ifndef MASHUP_COMMON_INSPECTOR_MASHUPINSPECTOR_
#define MASHUP_COMMON_INSPECTOR_MASHUPINSPECTOR_

#include <string>
#include <iostream>

#include "../../model/Mashup.h"

/**
 * @brief Integrity of data analysis for Mashup objects.
 *
 * This class might be used to query for information about
 * a given Mashup instance or to friendly display their
 * properties.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class MashupInspector {
  public:
    /**
     * Builds a pretty-formatted string using the properties
     * on a given Mashup instance.
     * @param mashup Mashup instance.
     * @return Pretty-formatted string.
     */
    static std::string Print(const Mashup& mashup);
    /**
     * Builds a pretty-formatted stringstream using the properties
     * on a given Mashup instance.
     * @param mashup Mashup instance.
     * @param out    Stream to build the string.
     * @param indent Identation spaces.
     */
    static void Print(const Mashup& mashup, std::stringstream& out, int indent = 0);
    /**
     * Builds a pretty-formatted stringstream using the properties
     * on a given AssetDescriptor instance.
     * @param asset_desc AssetDescriptor instance.
     * @param out        Stream to build the string.
     * @param indent Identation spaces.
     */
    static void Print(const AssetDescriptor& asset_desc,
                      std::stringstream& out, int indent = 0);
    /**
     * Builds a pretty-formatted stringstream using the properties
     * on a given AssetQualityDescriptor instance.
     * @param asset_quality_desc AssetQualityDescriptor instance.
     * @param out                Stream to build the string.
     * @param indent Identation spaces.
     */
    static void Print(const AssetQualityDescriptor& asset_quality_desc,
                      std::stringstream& out, int indent = 0);
    /**
     * Builds a pretty-formatted stringstream using the properties
     * on a given Scene instance.
     * @param scene Scene instance.
     * @param out   Stream to build the string.
     * @param indent Identation spaces.
     */
    static void Print(const Scene& scene,
                      std::stringstream& out, int indent = 0);
    /**
     * Builds a pretty-formatted stringstream using the properties
     * on a given Actor instance.
     * @param actor Actor instance.
     * @param out   Stream to build the string.
     * @param indent Identation spaces.
     */
    static void Print(const Actor& actor,
                      std::stringstream& out, int indent = 0);
    /**
     * Builds a pretty-formatted stringstream using the properties
     * on a given Transition instance.
     * @param transition Transition instance.
     * @param out        Stream to build the string.
     * @param indent Identation spaces.
     */
    static void Print(const Transition& transition,
                      std::stringstream& out, int indent = 0);

  private:
    /**
     * Private class constructor.
     */
    MashupInspector();
};

#endif // MASHUP_COMMON_INSPECTOR_MASHUPINSPECTOR_
