#include "appinfo.h"

#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "model/Mashup.h"
#include "common/parser/MashscriptJsonParser.h"
#include "util/Log.h"

int main(int argc, char *argv[]) {
  try {
    std::string inputFile;
    std::string outputFile;
    std::string outputFormat;
    po::options_description genericOpts("Generic options");
    genericOpts.add_options()
      ("help,h", "produce help message.")
      ("version", "print the version number.")
      ("verbose,v", "verbosity enabled to level TRACE and DEBUG.")
    ;
    po::options_description configShownOpts("Configuration");
    configShownOpts.add_options()
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
      std::cout << printOpts << "\n";
      return 1;
    }

    if (vm.count("input-file")) {
      std::cout << "Input file set to "
        << vm["input-file"].as<std::string>() << "\n";
    } else {
      std::cout << "Input file not set." << "\n";
    }

    if (vm.count("verbose")) {
      Log::SetLevel(Log::TRACE);
    }
    Mashup* mashup = new Mashup();
    MashscriptJsonParser* jsonParser = new MashscriptJsonParser();
    jsonParser->FromFile(inputFile, *mashup);
  } catch(std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  } catch(...) {
    std::cerr << "Exception of unknown type!\n";
  }

  return 0;
}
