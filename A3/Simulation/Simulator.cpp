#include "../Common/Simulator.h"

// Number of fields contained in the input file, excluding the map
const int number_of_fields = 5; // name, max battery, max steps, num rows, num cols
const std::vector<std::string> data_names = {"MaxSteps", "MaxBattery", "Rows", "Cols"};

House Simulator::readHouseFile(const std::string &houseFileMap, const std::string &houseName) {

    // Lambda to check if a string is a number
    auto isNumber = [](std::string &s) {
        s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
        return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
    };

    std::string outputFileName = houseFileMap;      // TODO: MUST FIX, OUTPUT FILE NAME CHANGES PER PERMUTATION
    size_t pos = outputFileName.find_last_of(".");
    if (pos != std::string::npos && outputFileName.substr(pos) == ".house") {
        outputFileName.replace(pos, 4, "Output.txt");
    } else {
        throw std::runtime_error("Invalid file format " + houseFileMap + ". Must be file with extension '.house'");
    }

    // Read file as input file stream inf
    std::ifstream inf{ houseFileMap };

    // File could not be opened
    if (!inf) {
        inf.close();
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
        throw std::runtime_error("Input file does not contain a map.");
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
    outputFile = outputFileName;        // TODO: FIX SO OUTPUTFILENAME IS SET PER PERMUTATION USING ALGO
    maxBattery = simulation_information[1];
    batteryState = maxBattery;
    return House(dirt_level, simulation_information[2], simulation_information[3], docking_location, docking_location, map, houseName);
}

void Simulator::readHouses(const std::string &housePath) {
    for (const auto &entry : std::filesystem::directory_iterator(housePath)) {
        std::filesystem::path path = entry.path();
        if (path.string().substr(path.string().find_last_of(".") + 1) == "house") {
            houses.push_back(readHouseFile(path.string(), path.stem()));
        }
    }
}

void Simulator::loadAlgorithms(const std::string &algoPath) {
    for (const auto &entry : std::filesystem::directory_iterator(algoPath)) {
        std::string path = entry.path().string();
        std::string baseFilename = path.substr(path.find_last_of("/\\") + 1);
        void *algoHandler;
        if(algoHandler = dlopen(path.c_str(), RTLD_LAZY)){
            algorithmNames.push_back(baseFilename);
            algorithmHandlers.push_back(algoHandler);
        } 
    }

    for(const auto& algo: AlgorithmRegistrar::getAlgorithmRegistrar()) {
        auto algorithm = algo.create();
        algorithms.push_back(std::move(algorithm));
    }

}

void Simulator::unloadAlgorithms() {
    AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    for (auto&& handler : algorithmHandlers) {
        if(dlclose(handler)) {
            // Did not dlclose properly
        }
    }
    // algorithmNames.clear();
    // algorithmHandlers.clear();
    // algorithms.clear();
}


// NEEDS TO BE HANDLED, OUTPUT FILE NAME CHANGES PER PERMUTATION
void Simulator::generateOutputFile() {
    // Set outputstream
    std::ofstream output;
    // Open file for ostream
    output.open(outputFile);

    int score = 0;
    // Grab status
    std::string status = "";
    if (currHouse->isDocked()) {
        status = "FINISHED";
        score = currHouse->getMaxSteps() + currHouse->getTotalDirt() * 300 + 3000;
    } else {
        if (batteryState == 0) {
            status = "DEAD";
            score = currHouse->getMaxSteps() + currHouse->getTotalDirt() * 300 + 2000;
        } else {
            status = "WORKING";
            score = movesTaken.size() + currHouse->getTotalDirt() * 300 + (currHouse->isDocked() ? 0: 1000);
        }
    }


    // Convert data to string format and insert 
    output << "NumSteps = " << movesTaken.size() << std::endl;
    output << "DirtLeft = " << currHouse->getTotalDirt() << std::endl;
    output << "Status = " << status << std::endl;
    output << "inDock = " << currHouse->isDocked() << std::endl;
    output << "Score = " << score << std::endl;
    output << "Steps:" << std::endl;
    // Take moves in loop
    for (auto i : movesTaken) {
        output << i;
    }

    // close file
    output.close();
}

bool Simulator::isWall(Direction d) const {
    // Initialize directions as N E S W
    return currHouse->isWall(d);
}

int Simulator::dirtLevel() const {
    // Calculate current position
    return currHouse->dirtLevel();
}

std::size_t Simulator::getBatteryState() const {
    // Simple return for battery
    return batteryState;
}

void Simulator::run() {
    for (auto &algorithm : algorithms) {
        for (auto &house : houses) {
            setAlgorithm(*algorithm);
            setHouse(house);
            // TODO: SET OUTPUT FILE NAME BASED ON VECTOR OF ALGORITHMNAMES AND house.getName()
            runPair();
            // CSV Output stuff if needed
        }
    }
}

void Simulator::runPair() {

    std::vector<char> stepToChar = {'N', 'E', 'S', 'W', 's', 'F'};
    

    // Start off maxIterations counter
    int max = currHouse->getMaxSteps();
    int iterations = 0;
    while (iterations < max) {
        // Display Map
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
                partialCharge += maxBattery/20.;

                // If the partialCharge + the current batteryState = maxBattery, battery is fully charged
                if (partialCharge + batteryState == maxBattery) {
                    batteryState = maxBattery;
                    partialCharge = 0;
                }
            }
            // Call Simulator::handleMove once again
            currHouse->handleMove(nextStep);
        } else if (nextStep == Step::Finish) {
            // If finishing, then output and break
            std::cout << "Algorithm has finished" << std::endl;
            break;
        } else {
            // Otherwise, handle whichever move is being made
            currHouse->handleMove(nextStep);
        }
        // Iterate iterations
        ++iterations;
    }
    
    generateOutputFile();
}

void Simulator::displayMap(int x) const {
    currHouse->displayHouse(x);
}