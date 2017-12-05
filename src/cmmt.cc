 /**
  * @file
  * CMMT application.
  *
  * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
  */

#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "filesystem.h"
#include "cmmt_exception.h"
#include "project.h"

namespace po = boost::program_options;


/**
 * Convert the paths given on the command line to a set of std::filesystem:path
 * objects referencing regular files.
 *
 * @param base_paths
 *
 * @return  A set of std::filesystem::path objects referencing regular files.
 */
auto get_files(const std::vector<std::string>& base_paths)
{
    paths_t paths;

    for (auto& p: base_paths) {
        auto path = std::filesystem::path(p);
        auto status = std::filesystem::status(path);
        if (!std::filesystem::exists(status)) {
            std::cerr << "Path not found: " << path << std::endl;
            return paths;
        } else if (std::filesystem::is_regular_file(status)) {
            paths.emplace(std::move(path));
        } else if (std::filesystem::is_directory(status)) {
            for (auto it = std::filesystem::recursive_directory_iterator(path);
                 it != std::filesystem::recursive_directory_iterator();
                 ++it) {
                auto dentry = *it;

                if (dentry.path().filename().string()[0] == '.') {
                    if (std::filesystem::is_directory(dentry.status())) {
                        it.disable_recursion_pending();
                    }
                } else if (std::filesystem::is_regular_file(dentry.status())) {
                    paths.insert(dentry);
                }
            }
        }
    }

    return paths;
}


/**
 * Process command line options.
 *
 * @param argc  Number of command line arguments.
 * @param argv  The command line arguments.
 *
 * @return  The boost::program_options variables map.
 */
auto process_options(int argc, char *argv[])
{
    auto visible = po::options_description("cmmt [OPTIONS] [<path>...]");
    visible.add_options()
            ("help,h", "This help text.");

    auto hidden = po::options_description("Hidden options");
    hidden.add_options()
            ("path", po::value<std::vector<std::string>>(), "Paths to search for files.");


    auto cmdline_options = po::options_description("Command line options");
    cmdline_options.add(visible).add(hidden);

    auto pod = po::positional_options_description();
    pod.add("path", -1);

    po::variables_map vm;
    auto parser = po::command_line_parser(argc, argv);
    parser.options(cmdline_options);
    parser.positional(pod);
    po::store(parser.run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        class help_exception: public std::exception {
          public:
            help_exception(const std::string& help): _help(help) {}
            const char* what() const noexcept override { return _help.c_str(); }

          private:
            std::string _help;
        };

        std::ostringstream os;
        os << visible;
        throw help_exception(os.str());
    }

    return vm;
}


/**
 * Program main().
 *
 * @param argc  Number of command line arguments.
 * @param argv  The command line arguments.
 *
 * @return  Program termination code:
 *          - 0 On success.
 *          - 1 If any score exceeds the given threshold.
 *          - 2 Parsing error.
 *          - 3 All other errors (i.e., file access error).
 */
int main(int argc, char *argv[])
{
    try {
        auto vm = process_options(argc, argv);

        auto files = get_files((vm.count("path") ?
                                vm["path"].as<std::vector<std::string>>() :
                                std::vector<std::string>({ "." })));

        if (files.empty()) {
            return 3;
        }

        auto proj = project();
        proj.process_files(files);
        proj.report_text(std::cout);

    } catch (cmmt_exception& e) {
        std::cerr << e.what() << std::endl;
        return 2;
    }

    return 0;
}
