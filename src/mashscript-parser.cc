#include "appinfo.h"

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "model/Mashup.h"
#include "common/parser/MashscriptJsonParser.h"
#include "common/inspector/MashupInspector.h"
#include "util/Log.h"

int main(int argc, char *argv[]) {
  try {
    std::string inputFile;
    std::string outputFile;
    std::string outputFormat;
    std::string assets_path;
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
      ("output-file,o", po::value<std::string>(&outputFile)->default_value(""),
        "path to output smashscript file.")
      ("output-format,f", po::value<std::string>(&outputFormat)->default_value("json"),
        "print to the selected format. Allowed values are: \"json\" \"xml\"");
    ;
    po::options_description configHiddenOpts("Hidden options");
    configHiddenOpts.add_options()
      ("input-file,i", po::value<std::string>(&inputFile),
        "path to smashscript input file.")
    ;
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

    if (vm.count("verbose")) {
      Log::SetLevel(Log::TRACE);
    }
    Mashup* mashup = new Mashup();
    MashscriptJsonParser* jsonParser = new MashscriptJsonParser();
    jsonParser->assets_path = assets_path;
    jsonParser->FromFile(inputFile, *mashup);
    std::cout << MashupInspector::Print(*mashup);
  } catch(std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  } catch(...) {
    std::cerr << "Exception of unknown type!\n";
  }

  return 0;
}
