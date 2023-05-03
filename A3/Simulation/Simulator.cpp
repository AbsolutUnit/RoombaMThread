#include "../Common/Simulator.h"

thread_local AbstractAlgorithm *Simulator::currAlgo = nullptr;
thread_local House *Simulator::currHouse = nullptr;
thread_local std::size_t Simulator::batteryState = 0;
thread_local float Simulator::partialCharge = 0.0f;
thread_local std::vector<char> Simulator::movesTaken;

// Number of fields contained in the input file, excluding the map
const int number_of_fields = 5; // name, max battery, max steps, num rows, num cols
const std::vector<std::string> data_names = {"MaxSteps", "MaxBattery", "Rows", "Cols"};

// Lambda to check if a string is a number
auto isNumber = [](std::string &s) {
    s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
};

House Simulator::readHouseFile(const std::string &houseFileMap, const std::string &houseName) {

    std::string outputFileName = houseFileMap;      // TODO: MUST FIX, OUTPUT FILE NAME CHANGES PER PERMUTATION
    size_t pos = outputFileName.find_last_of(".");
    if (pos != std::string::npos && outputFileName.substr(pos) == ".house") {
        outputFileName.replace(pos, 4, "Output.txt");
    } else {
        std::ofstream err{houseFileMap + ".error"};
        err << "Invalid file format " + houseFileMap + ". Must be file with extension '.house'.\n";
        err.close();
        throw std::runtime_error("Invalid file format " + houseFileMap + ". Must be file with extension '.house'.\n");
    }

    // Read file as input file stream inf
    std::ifstream inf{ houseFileMap };

    // File could not be opened
    if (!inf) {
        std::ofstream err{houseFileMap + ".error"};
        err << "Couldn't open " + houseFileMap + " for reading.\n";
        err.close();
        throw std::runtime_error("Couldn't open " + houseFileMap + " for reading.\n");
    }
  
    std::vector<std::string> tokens;
    std::string input;
    
    // Tokenize file 
    std::getline(inf, input);
    if (input[input.size() - 1] == '\r') // Remove carriage returns
        input.erase(input.size() - 1);
    while (inf) { // While buffer is not EOF
        tokens.push_back(std::regex_replace(input, std::regex("="), " = ")); //replace all '=' with " = " for easy delimitation
        // tokens.push_back(input);
        std::getline(inf, input);
        if (input[input.size() - 1] == '\r')
            input.erase(input.size() - 1);
    }
    
    // File contains fewer than than does not contain map
    if (tokens.size() < number_of_fields) { 
        std::ofstream err{houseFileMap + ".error"};
        err << "Input file does not contain a map.\n";
        err.close();
        throw std::runtime_error("Input file does not contain a map.\n");
    }

    std::vector<int> simulation_information; // max_steps, max_battery, num rows, num cols

    // Verify the input file is in the correct format
    std::string s;
    std::vector<std::string> line_info;
    for (int i = 1; i < number_of_fields; ++i) {
        std::stringstream ss(tokens[i]);
        while (std::getline(ss, s, '=')) {
            s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
            line_info.push_back(s); // Tokenize line by '='
        }
        // A line contains more than 3 tokens,
        // or the first token does not match expected, and second token is not a number
        if (line_info.size() != 2 || data_names[i - 1] != line_info[0] 
            || !isNumber(line_info[1])) {
            inf.close();
            std::ofstream err{houseFileMap + ".error"};
            err << "Input file is not formatted correctly.";
            err.close();
            throw std::runtime_error("Input file is not formatted correctly.");
        }
        simulation_information.push_back(std::stoi(line_info[1]));
        line_info.clear();
    }

    // Close input file stream
    inf.close();

    // Extract relevant information from input file 
    Point docking_location {-1, -1};
    int dirt_level = 0;
    std::vector<char> map;

    bool dockFound = false;
    int counted_rows = 0;
    // Traverse each line in the map until the minimum of the number of lines or the Rows value
    for (int i = 0; i < std::min(int(tokens.size()) - number_of_fields, simulation_information[2]); ++i) {
        int counted_columns = 0;
        // Traverese each character in each line in the map until the minimum of the number of characters in a line or the Cols value
        for (int j = 0; j < std::min(int(tokens[i + number_of_fields].size()), simulation_information[3]); ++j) { // traverse each char in each string
            // Set location of dock
            if (tokens[i + number_of_fields][j] == DOCK) {
                if (dockFound) {
                    std::ofstream err{houseFileMap + ".error"};
                    err << "Map contains two docks!";
                    err.close();
                    throw std::runtime_error("Map contains two docks!");
                }
                docking_location.y = i;
                docking_location.x = j;
                dockFound = true;
            }
            // If the current cell contains a digit, add it to the total dirt level.
            if (isdigit(tokens[i + number_of_fields][j])) {
                dirt_level += int(tokens[i + number_of_fields][j]) - 48;
            }

            map.push_back(tokens[i + number_of_fields][j]);
            ++counted_columns;
        }
        // Pad extra columns to the right, if the counted number of columns is less than Cols
        while (counted_columns < simulation_information[3]) {
            map.push_back('0');
            ++counted_columns;
        }
        ++counted_rows;
    }

    if (!dockFound) {
        // Throw error if no dock is there
        std::ofstream err{houseFileMap + ".error"};
        err << "Dock is missing!";
        err.close();
        throw std::runtime_error("Dock is missing!");
    }

    // Pad extra rows below, if the counted number of rows is less than Rows
    while (counted_rows < simulation_information[2]) {
        for (int i = 0; i < simulation_information[3]; ++i) {
            map.push_back('0');
        }
        ++counted_rows;
    }

    // Communicate data to variables within Simulator
    return House(dirt_level, simulation_information[0], simulation_information[1], simulation_information[2], simulation_information[3], docking_location, docking_location, map, houseName);
}

void Simulator::readHouses(const std::string &housePath) {
    // Iterate through house path entries
    for (const auto &entry : std::filesystem::directory_iterator(housePath)) {
        // Grab the file path from the iterations
        std::filesystem::path path = entry.path();
        // Check if suffix is house
        if (path.string().substr(path.string().find_last_of(".") + 1) == "house") {
            try
            {
                // If so, try to readHouse file (may throw exception)
                houses.push_back(readHouseFile(path.string(), path.stem()));
            }
            catch(const std::exception& e)
            {
                // If house file invalid, skip and continue
                continue;
            }
        }
    }
}

void Simulator::loadAlgorithms(const std::string &algoPath) {
    // Iterate through all files in the directory given by algoPath
    int algorithmCount = AlgorithmRegistrar::getAlgorithmRegistrar().count();
    for (const auto &entry : std::filesystem::directory_iterator(algoPath)) {
        // Get path variable
        std::string path = entry.path().string();
        // get root of folder
        std::string baseFilename = entry.path().stem().string().substr(entry.path().stem().string().find_last_of("/\\") + 1);
        void *algoHandler;
        // If the algo does not end with .so, skip it
        if (entry.path().extension().string() != ".so") { // If the current file is not a .so file skip
            continue;
        }
        // If it cannot dlopen, skip it
        if (!(algoHandler = dlopen(path.c_str(), RTLD_LAZY))){ // If failure to dlopen .so file, write to error file
            std::ofstream err{baseFilename + ".error"};
            err << "Error loading algorithm library: " << dlerror() << std::endl;
            err.close();
            continue;
        } 
        // Increment independent algo count
        algorithmCount++;
        // If the independent algoCount and current count (which is the new Registrar count) are not equivalent then continue
        if (AlgorithmRegistrar::getAlgorithmRegistrar().count() != algorithmCount) {
            // We due this since this means an algo was not valid/did not call the MACRO
            algorithmCount--;
            std::ofstream err{baseFilename + ".error"};
            err << "Error loading algorithm library: " << dlerror() << std::endl;
            // Give an error file for the error of the algorithm.
            err.close();
            continue;
        }
        algorithmHandlers.push_back(algoHandler);

    }

    registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
    // Grab the registrar
    for(const auto& algo: AlgorithmRegistrar::getAlgorithmRegistrar()) {    // Get algorithm pointers and populate data structures
        auto algorithm = algo.create();
        // Check if algo can be created
        if (algorithm == nullptr) {
            std::string fname = (algo.name() == "")? algo.name(): "algorithm";
            std::ofstream err{fname + ".error"};
            err << "Error loading algorithm library: " << dlerror() << std::endl;
            err.close();
            continue;
        }
        // If not, skip, if it can be created add names to name list
        algorithmNames.push_back(algo.name());
        // algorithms.push_back(std::move(algorithm));
    }
}

void Simulator::unloadAlgorithms() {
    // Iterate through algo handlers
    for (auto&& handler : algorithmHandlers) {
        // clear the algo registrar
        AlgorithmRegistrar::getAlgorithmRegistrar().clear();
        // Local copy clear
        registrar.clear();
        // Dlclose, and if failed continue through other algos
        if(dlclose(handler)) {
            continue;
        }
    }
}

std::unique_ptr<AbstractAlgorithm> Simulator::createAlgorithmByName(std::string name) {
    // Find the index of algo in registrar 
    auto it = std::find_if(registrar.begin(), registrar.end(), [&](auto pair) {
        return name == pair.name();
    });
    
    // If valid index, return the created algo
    if (it != registrar.end()) {
        return (*it).create();
    }
    // Otherwise return nullptr
    return nullptr;
}

int Simulator::calculateScore(Status status) {
    // Check conditions
    if (status == Status::DEAD)
        return currHouse->getMaxSteps() + currHouse->getTotalDirt() * 300 + 2000; // Dead
    if (status == Status::FINISHED && !currHouse->isDocked()) 
        return currHouse->getMaxSteps() + currHouse->getTotalDirt() * 300 + 3000; // Finished not on dock

    return movesTaken.size() + currHouse->getTotalDirt() * 300 + (currHouse->isDocked() ? 0: 1000); // Otherwise 
}

int Simulator::generateOutputFile(std::string algoName) {
    // Set outputstream
    std::ofstream output;

    // Grab status
    Status status;
    std::vector<std::string> statuses = {"FINISHED", "WORKING", "DEAD"};
    // Initialize steps taken var
    int stepCount = 0;
    // Check for last move being Finished
    if (currHouse->isDocked() && movesTaken[movesTaken.size() - 1] == 'F') {
        // If so, status is finished
        status = Status::FINISHED;
        // Subtract finished step for calculation
        stepCount = movesTaken.size() - 1;
    } else {
        if (batteryState == 0) {
            // Otherwise set status and take all steps
            status = Status::DEAD;
            stepCount = movesTaken.size();
        } else {
            // Same here
            status = Status::WORKING;
            stepCount = movesTaken.size();
        }
    }

    // Get score calculation
    int score = calculateScore(status);

    // If not summary flag (regular run)
    if (!summaryFlag){
        // Open file for ostream
        output.open(currHouse->getName() + "-" + algoName + ".txt");
        // Convert data to string format and insert 
        output << "NumSteps = " << stepCount << std::endl;
        output << "DirtLeft = " << currHouse->getTotalDirt() << std::endl;
        output << "Status = " << statuses[static_cast<int>(status)] << std::endl;
        output << "inDock = " << ((currHouse->isDocked())? "TRUE": "FALSE") << std::endl;
        output << "Score = " << score << std::endl;
        output << "Steps:" << std::endl;
        // Take moves in loop
        for (auto i : movesTaken) {
            output << i;
        }

        // close file
        output.close();
    }
    // Return score either way
    return score;
}


void Simulator::generateSummary() const {
    // Open output file for summary.csv
    // std::cout << "Hitting generate summ " << std::endl;
    std::ofstream output;
    output.open("summary.csv");

    // first row corresponding to algo names
    output << "Algorithms/Houses,";
    for (auto x : houses) {
        output << x.getName() << ",";
    }
    output << '\n';

    for (int j = 0; j < algorithmNames.size(); ++j) {
        // Iterate rows and cols adding relevant data
        output << algorithmNames[j] << ',';
        for (int i = 0; i < houses.size(); ++i) {
            // std::string x = houses[j].houseName + "-" + algorithmNames[i];
            // output << scores.at(x) << ',';
            output << scores[i * algorithmNames.size() + j] << ',';
        }
        output << '\n';
    }
    // Close output
    output.close();
}

bool Simulator::isWall(Direction d) const {
    // check if house valid
    if (currHouse == nullptr) {
        std::cerr << "Error: currHouse pointer is null." << std::endl;
        return true;
    }
    // Call House function
    return currHouse->isWall(d);
}

int Simulator::dirtLevel() const {
    // check if house valid
    if (currHouse == nullptr) {
        std::cerr << "Error: currHouse pointer is null." << std::endl;
        return 0;
    }
    // Call House function
    return currHouse->dirtLevel();
}

std::size_t Simulator::getBatteryState() const {
    // Simple return for battery
    return batteryState;
}

void Simulator::run() {
    // std::cout << "Thread count set as: " << threadCount << std::endl;
    // std::cout << "Charging rate denom set as: " << chargingConstant << std::endl;
    // Initialize thread pool for values
    ThreadPool pool = ThreadPool(threadCount);
    std::cout << "Running simulatons with " << threadCount << " threads" << std::endl;
    // Loop througb algos
    for (int i = 0; i < algorithmNames.size(); ++i) {
        // And loop houses
        for (int j = 0; j < houses.size(); ++j) {
            // Get Algo name to prevent race conditions
            std::string algoName = algorithmNames[i];
            // Queue job
            pool.enqueue([this, i, j, house = House(houses[j]), algo = createAlgorithmByName(algorithmNames[i]), algoName] {
                // copy House
                House houseCopy(house);
                // set to the house copy
                setHouse(houseCopy);
                // Set algo to newly created algo ptr
                setAlgorithm(*algo);

                try
                {
                    // Try running pair using runPair
                    scores[i * houses.size() + j] = runPair(algoName);
                }
                catch(const std::exception& e)
                {
                    std::cout << e.what() << '\n';
                }
            });
            // std::cout << "Added task to queue" << std::endl;
        }
    }
    // std::cout << "Added all to queue" << std::endl;
    // Await all thread pool task completion
    pool.wait();
    // std::cout << "Awaited all" << std::endl;
    // Generate summary csv
    generateSummary();
}

int Simulator::runPair(std::string algoName) {

    // Initialize all directions
    std::vector<char> stepToChar = {'N', 'E', 'S', 'W', 's', 'F'};
    // Doube check that movesTaken is clear (since thread may do multiple jobs over lifetime)
    clearCache();

    // Check valid house
    if (currHouse == nullptr) {
        std::cerr << "Error: currHouse pointer is null." << std::endl;
        throw std::runtime_error("currHouse is null!");
    }

    // Check valid algo
    if (currAlgo == nullptr) {
        std::cerr << "Error: currAlgo pointer is null." << std::endl;
        throw std::runtime_error("currAlgo is null!");
    }
    

    // Start off maxIterations counter
    int max = currHouse->getMaxSteps();
    // Start timer for timeout
    auto startTime = std::chrono::steady_clock::now();
    int iterations = 0;
    while (iterations < max) {
        // Check current time
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        // If enough time has elapsed
        if (static_cast<int>(elapsed.count()) >= 1) {
            // Timeout has elapsed, return default score
            return (max * 2) + (currHouse->getInitialDirt() * 300) + 2000;
        }

        // If display flag set, display house
        if (displayFlag){
            displayMap(iterations);
            std::cout << "Steps left: " << max - iterations << std::endl;
        }

        // Get nextStep from Algo
        Step nextStep = currAlgo->nextStep();
        movesTaken.push_back(stepToChar[static_cast<int>(nextStep)]);

        // Separate logic for stay
        if (nextStep == Step::Stay) {
            // If docking, charge and do multiple steps at once
            if (currHouse->isDocked()) {
                // Keep track of charge in a float
                partialCharge += currHouse->getMaxBattery()/chargingConstant;

                // If the partialCharge + the current batteryState = maxBattery, battery is fully charged
                if (partialCharge + batteryState >= currHouse->getMaxBattery()) {
                    batteryState = currHouse->getMaxBattery();
                    partialCharge = 0;
                }
            }
            // Call Simulator::handleMove once again
            currHouse->handleMove(nextStep);
        } else if (nextStep == Step::Finish) {
            // If finishing, then output and break
            // std::cout << "Algorithm has finished" << std::endl;
            break;
        } else {
            // Otherwise, handle whichever move is being made
            --batteryState;
            currHouse->handleMove(nextStep);
        }
        // Iterate iterations
        ++iterations;
    }
    // TODO: for summary, return score?
    return generateOutputFile(algoName);
}

void Simulator::displayMap(int x) const {
    // Check for house validity
    if (currHouse == nullptr) {
        std::cerr << "Error: currHouse pointer is null." << std::endl;
        return;
    }

    // Call house func
    currHouse->displayHouse(x);
}

void Simulator::clearCache() {
    movesTaken.clear();
}