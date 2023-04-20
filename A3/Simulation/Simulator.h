//
// Created by Anshuman Funkwal on 3/13/23.
//

#pragma once

#include <vector>
#include <string>
#include "../Common/House.h"
#include "../Common/AbstractAlgorithm.h"
#include "../Common/common_types.h"
#include "../Common/common_structs.h"

// You may want to add additional classes for simulation but you should have a Simulator class.
// You can use the below suggestion or alternatively implement concrete class for each sensor.
class Simulator : public WallsSensor, public DirtSensor, public BatteryMeter {
    std::size_t maxSteps = 0;
    std::size_t batteryState, maxBattery = 0;
    std::string outputFile = "";
    bool displayFlag = true;
    float partialCharge = 0;
    std::vector<char> movesTaken;
    std::vector<House> houses;
    std::vector<AbstractAlgorithm> algorithms;
    AbstractAlgorithm *currAlgo;
    House *currHouse;
public:

    Simulator(const std::string &housePath, const std::string &algoPath, bool display)
    {
        readHouses(housePath);

    };

    void run(){};

    void runPair(){};

    ~Simulator(){};

private:
    void setAlgorithm(AbstractAlgorithm &algorithm) {
        currAlgo = &algorithm;
        currAlgo->setMaxSteps(maxSteps);
        currAlgo->setWallsSensor(*this);
        currAlgo->setDirtSensor(*this);
        currAlgo->setBatteryMeter(*this);
    }

    void setHouse(House &house) {
        currHouse = &house;
    }

    bool isWall(Direction d) const override {};

    int dirtLevel() const override {};

    std::size_t getBatteryState() const override {};

    void readHouses(const std::string &housePath) {
        // loop through folder and call readHouseFile per .house
        // pushBack to vector of houses
    };

    House readHouseFile(const std::string &houseFilePath);

    void loadAlgorithms(const std::string &algoPath) {
        // loop through folder and dlopen each .so file
        // pushBack to vector of algorithms
    };

    int convertToCoordinates(Point p) const;

    void handleMove(Step s, int location);

    void displayMap(int iterations) const;

    void generateOutputFile();
};

// #pragma once

// #include <string>
// #include "../Common/AbstractAlgorithm.h"

// // You may want to add additional classes for simulation but you should have a Simulator class.
// // You can use the below suggestion or alternatively implement concrete class for each sensor.
// class Simulator : public WallsSensor, public DirtSensor, public BatteryMeter {

// public:







// };
