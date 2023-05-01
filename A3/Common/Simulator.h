//
// Created by Anshuman Funkwal on 3/13/23.
//

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <dlfcn.h>
#include <map>
#include <regex>
#include <filesystem>
#include <thread>

#include "../Common/House.h"
#include "../Common/AbstractAlgorithm.h"
#include "../Common/common_types.h"
#include "../Common/common_structs.h"
#include "../Common/AlgorithmRegistrar.h"


// You may want to add additional classes for simulation but you should have a Simulator class.
// You can use the below suggestion or alternatively implement concrete class for each sensor.
class Simulator : public WallsSensor, public DirtSensor, public BatteryMeter {
public:
    std::size_t batteryState, maxBattery = 0;
    std::string outputFile = "";
    bool displayFlag = true;
    bool summaryOnly = false;
    float partialCharge = 0;
    std::vector<char> movesTaken;
    std::vector<House> houses;
    std::vector<std::unique_ptr<AbstractAlgorithm>> algorithms;
    std::map<std::string, std::unique_ptr<AbstractAlgorithm>> nameToAlgorithm;
    std::vector<std::string> algorithmNames;
    std::vector<void*> algorithmHandlers;
    std::unordered_map<std::string, int> scores;
    AbstractAlgorithm *currAlgo;
    House *currHouse;
// public:
    Simulator(const std::string &housePath, const std::string &algoPath, bool display)
    {
        readHouses(housePath);
        loadAlgorithms(algoPath);
        displayFlag = display;
    };

    void run();

    int runPair();

    ~Simulator(){
        for (auto&& algos : algorithms) {
            algos.reset();
        }
        unloadAlgorithms();
    };

private:
    void setAlgorithm(AbstractAlgorithm &algorithm) {
        currAlgo = &algorithm;
        currAlgo->setMaxSteps(currHouse->getMaxSteps());
        currAlgo->setWallsSensor(*this);
        currAlgo->setDirtSensor(*this);
        currAlgo->setBatteryMeter(*this);
    }

    void setHouse(House &house) {
        currHouse = &house;
    }

    bool isWall(Direction d) const override;

    int dirtLevel() const override;

    std::size_t getBatteryState() const override;

    void readHouses(const std::string &housePath);
        // loop through folder and call readHouse on each file
        // Remember to grab name since readHouseFile now takes in name
        // Thats it

    House readHouseFile(const std::string &houseFilePath, const std::string &houseName);

    void loadAlgorithms(const std::string &algoPath);

        // Loop through folder
        // Dlopen each so file
        // Handle errors if there
        // Add algorithm name via pushback

        // Recommendation: Make this a helper function
        // THIS THIS THIS THIS
        // For auto algo in AlgorithmRegistrar::getAlgorithmRegistrar()
        // Get algo via algo.create()
        // Pushback to vector of algorithms
        // THIS THIS THIS THIS

        // Dlclose each algo file
        // Options are call per iteration of folder loop
        // Or do loop for folder, loop for pushback algo, loop for folder again dlclosing
        

    void unloadAlgorithms();

    int calculateScore(Status status);

    void displayMap(int iterations) const;

    int generateOutputFile();

    void generateSummary() const;
};
