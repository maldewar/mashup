#include "appinfo.h"

#include <iostream>

#include <gst/gst.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "model/Mashup.h"
#include "common/parser/MashscriptJsonParser.h"
#include "common/inspector/MashupInspector.h"
#include "pipeline/Pipeline.h"
#include "common/builder/PipelineBuilder.h"
#include "util/Log.h"

int main(int argc, char *argv[]) {
  try {
    std::string input_file;
    std::string assets_path;
    std::string gst_debug;
    po::options_description genericOpts("Generic options");
    genericOpts.add_options()
      ("help,h", "produce help message.")
      ("version", "print the version number.")
      ("verbose,v", "verbosity enabled to level TRACE and DEBUG.")
    ;
    po::options_description configShownOpts("Configuration");
    configShownOpts.add_options()
      ("assets-path,p", po::value<std::string>(&assets_path)->default_value(""),
        "path to prepend to the assets path description.")
      ("gst-debug", po::value<std::string>(&gst_debug)->default_value(""),
        "takes a comma-separated list of category_name:level pairs to set specific "
        "levels for the individual categories. Example: GST_AUTOPLUG:5,avidemux:3. "
        "Alternatively, you can also set the GST_DEBUG environment variable, "
        "which has the same effect.");
    po::options_description configHiddenOpts("Hidden options");
    configHiddenOpts.add_options()
      ("input-file,i", po::value<std::string>(&input_file),
        "path to smashscript input file.");

    // Commandline options.
    po::options_description cmdlineOpts;
    cmdlineOpts.add(genericOpts).add(configShownOpts).add(configHiddenOpts);
    po::options_description printOpts;

    // Commandline options displayed with help.
    printOpts.add(genericOpts).add(configShownOpts);
    po::positional_options_description p;
    p.add("input-file", -1);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(cmdlineOpts).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
      boost::filesystem::path p = argv[0];
      std::string exe_name = p.stem().string();
      std::cout << "Parser for Mashscript documents.\n";
      std::cout << "usage: " << exe_name << " [options] infile";
      std::cout << printOpts << "\n";
      return 0;
    }

    if (vm.count("version")) {
      std::cout << "Parser for Mashscript documents, version 1.0\n";
      return 0;
    }

    if (vm.count("input-file")) {
      std::cout << "Input file set to "
        << vm["input-file"].as<std::string>() << "\n";
    } else {
      std::cerr << "Input file not set." << "\n";
      return 1;
    }

    if (vm.count("gst-debug")) {
      setenv("GST_DEBUG", gst_debug.c_str(), 1);
    }

    if (vm.count("verbose")) {
      Log::SetLevel(Log::TRACE);
    }
    Mashup* mashup = new Mashup();
    MashscriptJsonParser* jsonParser = new MashscriptJsonParser();
    jsonParser->assets_path = assets_path;
    jsonParser->FromFile(input_file, *mashup);
    std::cout << MashupInspector::Print(*mashup);
    gst_init (&argc, &argv);
    ges_init ();
    Pipeline* pipeline = PipelineBuilder::Build(*mashup);
    pipeline->Play();
  } catch(std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  } catch(...) {
    std::cerr << "Exception of unknown type!\n";
  }

/*
  // CREATING A PIPELINE AND LINKING ELEMENTS WITHIN.
  GstElement *pipeline;
  GstElement *source, *filter, *sink;

  // init
  gst_init (&argc, &argv);

  // create pipeline
  pipeline = gst_pipeline_new ("my-pipeline");

  // create elements
  source = gst_element_factory_make ("fakesrc", "source");
  filter = gst_element_factory_make ("identity", "filter");
  sink = gst_element_factory_make ("fakesink", "sink");

  // must add elements to pipeline before linking them
  gst_bin_add_many (GST_BIN (pipeline), source, filter, sink, NULL);

  // link
  if (!gst_element_link_many (source, filter, sink, NULL)) {
    g_warning ("Failed to link elements!");
  }
*/


/*
  // DISPLAY INFORMATION FROM ELEMENTS
  GstElementFactory *factory;

  // init GStreamer
  gst_init (&argc, &argv);

  // get factory
  factory = gst_element_factory_find ("fakesrc");
  if (!factory) {
    g_print ("You don't have the 'fakesrc' element installed!\n");
    return -1;
  }

  // display information
  g_print ("The '%s' element is a member of the category %s.\n"
           "Description: %s\n",
           gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (factory)),
           gst_element_factory_get_metadata (factory, GST_ELEMENT_METADATA_KLASS),
           gst_element_factory_get_metadata (factory, GST_ELEMENT_METADATA_DESCRIPTION));

  return 0;
*/


/*
  // GET NAME OF ELEMENT
  GstElement *element;
  gchar *name;

  // init GStreamer
  gst_init (&argc, &argv);

  // create element
  element = gst_element_factory_make ("fakesrc", "source");

  // get name
  g_object_get (G_OBJECT (element), "name", &name, NULL);
  g_print ("The name of the element is '%s'.\n", name);
  g_free (name);

  gst_object_unref (GST_OBJECT (element));

  return 0;
*/


/*
  // GET ELEMENT FROM FACTORY
  GstElementFactory *factory;
  GstElement * element;

  // init GStreamer
  gst_init (&argc, &argv);

  // create element, method #2
  factory = gst_element_factory_find ("fakesrc");
  if (!factory) {
    g_print ("Failed to find factory of type 'fakesrc'\n");
    return -1;
  }
  element = gst_element_factory_create (factory, "source");
  if (!element) {
    g_print ("Failed to create element, even though its factory exists!\n");
    return -1;
  }

  gst_object_unref (GST_OBJECT (element));

  return 0;
*/


/*
  //GET ELEMENT 'WITHOUT' FACTORY
  GstElement *element;

  // init GStreamer
  gst_init (&argc, &argv);

  // create element
  element = gst_element_factory_make ("fakesrc", "source");
  if (!element) {
    g_print ("Failed to create element of type 'fakesrc'\n");
    return -1;
  }

  gst_object_unref (GST_OBJECT (element));

  return 0;
*/


/*
  // INITIALIZE GSTREAMER
  const gchar *nano_str;
  guint major, minor, micro, nano;

  gst_init (&argc, &argv);

  gst_version (&major, &minor, &micro, &nano);

  if (nano == 1)
    nano_str = "(CVS)";
  else if (nano == 2)
    nano_str = "(Prerelease)";
  else
    nano_str = "";

  printf ("This program is linked against GStreamer %d.%d.%d %s\n",
          major, minor, micro, nano_str);
  return 0;
*/
  
}
