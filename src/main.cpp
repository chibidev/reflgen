//
//  main.cpp
//  reflgen
//
//  Created by Zsolt Erhardt on 2018. 07. 12..
//  Copyright © 2018. Zsolt Erhardt. All rights reserved.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wassume"
#include <cppast/libclang_parser.hpp>
#pragma clang diagnostic pop
#include <cxxopts.hpp>

#include "reflgen/generators.h"

using namespace std::literals;

namespace program {
    static constexpr const char* const name = "reflgen";
    static constexpr const char* const description = "Generate reflection information about attributed classes";
    static constexpr const char* const version = "0.0.5";
}

int main(int argc, char* argv[]) {
    std::vector<std::string> filename;
    cxxopts::Options options(program::name,
                             program::name + " - "s + program::description + "\n"s);
    options.add_options()
        ("h,help", "display this help and exit")
        ("V,version", "display version information and exit")
        ("v,verbose", "be verbose when parsing")
        ("file", "file", cxxopts::value<std::vector<std::string>>(filename))
    ;
    options.add_options("compilation")
        ("d,dbpath", "set the directory where a 'compile_commands.json' file is located containing build information. Defaults to current directory.", cxxopts::value<std::string>()->default_value("."s))
    ;

    options.positional_help("file");

    options.parse_positional("file");
    auto&& result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help({"", "compilation"}) << std::endl;
        return 0;
    } else if (result.count("version")) {
        std::cout << program::name << " version " << program::version << std::endl;
        return 0;
    } else if (filename.empty()) {
        std::cerr << "missing file argument" << std::endl;
        return 1;
    }

    cppast::libclang_compilation_database database { result["dbpath"].as<std::string>() };
    cppast::libclang_compile_config config { database, filename.front() };
    cppast::cpp_entity_index idx;

    auto&& logger = (result.count("verbose")) ? cppast::default_verbose_logger() : cppast::default_logger();

    cppast::libclang_parser parser { logger };
    auto&& file = parser.parse(idx, filename.front(), config);
    if (!file) {
        std::cerr << "Parsing failed" << std::endl;
        return 2;
    }

    reflgen::generate_classes(*file);

    return 0;
}
