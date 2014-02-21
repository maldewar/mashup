#include "appinfo.h"

#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "common/parser/MashscriptJsonParser.h"

int main(int argc, char *argv[]) {
  try {
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message.")
      ("version,v", "print the version number.")
      ("input-file,i", po::value<std::string>(), "path to smashscript input file.")
      ("output-file,o", po::value<std::string>(), "path to output smashscript file.")
      ("compression,c", po::value<int>(), "set compression level.")
    ;
    po::positional_options_description p;
    p.add("input-file", -1);
    po::variables_map vm;        
    //po::store(po::parse_command_line(argc, argv, desc), vm);
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);    

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 1;
    }

    if (vm.count("compression")) {
      std::cout << "Compression level was set to " 
        << vm["compression"].as<int>() << ".\n";
    } else {
      std::cout << "Compression level was not set.\n";
    }
    if (vm.count("input-file")) {
      std::cout << "Input file set to "
        << vm["input-file"].as<std::string>() << "\n";
    } else {
      std::cout << "Input file not set." << "\n";
    }
  } catch(std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  } catch(...) {
    std::cerr << "Exception of unknown type!\n";
  }

  return 0;
  //std::cout << "Hello World!" << std::endl;
  //aMashscriptJsonParser* jsonParser = new MashscriptJsonParser();
  //jsonParser->ParseScript("");
}
