#include <iostream>
#include "../Common/Simulator.h"
// #include "AlgorithmCommon/MyAlgorithm.h"
#include "../Common/AlgorithmRegistrar.h"
#include <dlfcn.h>
#include <unistd.h>
#include <getopt.h>
#include <filesystem>


using AlgorithmPtr = std::unique_ptr<AlgorithmRegistrar>;

// getting command line arguments for the house file
int main(int argc, char **argv)
{

    // TODO: Create the simulator, move the code loading the algorithms into simulator class, 
    //       make sure you load all algorithms in the proper folder and not only two.

    // parse arguments

    int opt;
    int option_index = 0;
    const char* short_options = "";
    static struct option long_options[] = {
        {"house_path",      optional_argument, 0, 'h'},
        {"algo_path",       optional_argument, 0, 'a'},
        {"num_threads",     optional_argument, 0, 't'},
        {"display",         no_argument,       0, 'd'},
        {"summary_only",    no_argument,       0, 's'},
        {"logging",         no_argument,       0, 'l'},
        {0,                 0,                 0,  0}
    };

    std::string house_path, algo_path;
    int num_threads;
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
            case 'l':
                logging = true;
                break;
        }
    }

    std::cout << "house path: " << house_path << std::endl;
    std::cout << "algo path: " << algo_path << std::endl;

    Simulator simulator(house_path, algo_path, display, summary_only);

    std::cout << "MADE SIMULATOR" << std::endl;

    for (auto x : simulator.houses) {
        std::cout << x.houseName << std::endl;
    }

    std::cout << "algos" << std::endl;
    for (auto x: simulator.algorithmNames) {
        std::cout << x << std::endl;
    }

    simulator.run();

    // Algo_112710107
    // Algo_112628028

    std::cout << "UNLOAD" << std::endl;

    // TODO: PERFORM SCORE CALCULATION AND STORE, TO USE IN GENERATE OUTPUT FILES AND GENERATE SUMMARY FUNCTIONS

    // TODO: OUTPUT NAMING CONVENTION IN GENERATE OUTPUT FILES ALGO-HOUSE.TXT
    
    // TODO: RUNPAIR CALLS GENERATE OUTPUTFILE (RUNS 1 SIMULATION START TO FINISH)-> CALLED BY RUN()

    // // Load algorithm library
    // void *algorithm_handle1 = dlopen("Algorithm_1_123456789/libAlgorithm_1_123456789.so", RTLD_LAZY);
    // if (!algorithm_handle1)
    // {
    //     std::cerr << "Error loading algorithm library: " << dlerror() << std::endl;
    //     return 1;
    // }

    // void *algorithm_handle2 = dlopen("Algorithm_2_123456789/libAlgorithm_2_123456789.so", RTLD_LAZY);
    // if (!algorithm_handle2)
    // {
    //     std::cerr << "Error loading algorithm library: " << dlerror() << std::endl;
    //     AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    //     dlclose(algorithm_handle1);
    //     return 1;
    // }

    // for(const auto& algo: AlgorithmRegistrar::getAlgorithmRegistrar()) {
    //     auto algorithm = algo.create();
    //     std::cout << algo.name() << ": " << static_cast<int>(algorithm->nextStep()) << std::endl;
    // }

    // AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    // dlclose(algorithm_handle1);
    // dlclose(algorithm_handle2);
}
