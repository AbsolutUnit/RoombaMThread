#include <iostream>
#include <dlfcn.h>
#include <unistd.h>
#include <getopt.h>
#include <filesystem>

#include "../Common/Simulator.h"
#include "../Common/AlgorithmRegistrar.h"


using AlgorithmPtr = std::unique_ptr<AlgorithmRegistrar>;

// getting command line arguments for the house file
int main(int argc, char **argv)
{
    // Parse the Command Line Args
    int opt;
    int option_index = 0;
    const char* short_options = "";
    static struct option long_options[] = {
        {"house_path",      optional_argument, 0, 'h'},
        {"algo_path",       optional_argument, 0, 'a'},
        {"num_threads",     optional_argument, 0, 't'},
        {"display",         no_argument,       0, 'd'},
        {"summary_only",    no_argument,       0, 's'},
        {0,                 0,                 0,  0}
    };

    std::string house_path, algo_path;
    int num_threads = ARGNOTFOUND;
    bool summary_only, display, logging;
    summary_only = display = logging = false;
    house_path = algo_path = std::filesystem::current_path();

    while ((opt = getopt_long_only(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                house_path = optarg;
                break;
            case 'a':
                algo_path = optarg;
                break;
            case 't':
                num_threads = atoi(optarg);
                break;
            case 'd':
                display = true;
                break;
            case 's':
                summary_only = true;
                break;
        }
    }

    // Read the Configuration File if it exists
    std::unordered_map<std::string, int> results = readConfigFile(CONFIGFILEPATH);
    if (num_threads != ARGNOTFOUND && num_threads != 0)
        results[CONFIGPARAMS[1]] = num_threads;
    Simulator simulator(house_path, algo_path, display, summary_only, results);

    // Run the simulations
    simulator.run();

}