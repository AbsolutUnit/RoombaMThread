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

#include "../ThreadPool/ThreadPool.h"
#include "../Common/House.h"
#include "../Common/AbstractAlgorithm.h"
#include "../Common/common_types.h"
#include "../Common/common_structs.h"
#include "../Common/AlgorithmRegistrar.h"


// You may want to add additional classes for simulation but you should have a Simulator class.
// You can use the below suggestion or alternatively implement concrete class for each sensor.
class Simulator : public WallsSensor, public DirtSensor, public BatteryMeter {
public:
    static thread_local AbstractAlgorithm *currAlgo;
    static thread_local House *currHouse;
    static thread_local std::size_t batteryState;
    static thread_local float partialCharge;
    static thread_local std::vector<char> movesTaken;
    int threadCount = 10;
    double chargingConstant = 20;
    bool displayFlag = true;
    bool summaryFlag = false;
    std::vector<House> houses;
    std::map<std::string, std::unique_ptr<AbstractAlgorithm>> nameToAlgorithm;
    AlgorithmRegistrar registrar;
    std::vector<std::string> algorithmNames;
    std::vector<void*> algorithmHandlers;
    std::vector<int> scores;
// public:
    Simulator(const std::string &housePath, const std::string &algoPath, bool display, bool summary)
    {
        readHouses(housePath);
        loadAlgorithms(algoPath);
        displayFlag = display;
        summaryFlag = summary;
        scores = std::vector<int>(houses.size() * algorithmNames.size());
    };

    void run();

    int runPair(std::string algoName);

    ~Simulator(){
        for (auto &algoName : algorithmNames) {
            (nameToAlgorithm[algoName]).reset();
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
        partialCharge = 0.0;
        batteryState = currHouse->getMaxBattery();
    }

    bool isWall(Direction d) const override;

    int dirtLevel() const override;

    std::size_t getBatteryState() const override;

    void readConfigFile(std::string &configPath);
    
    void readHouses(const std::string &housePath);

    House readHouseFile(const std::string &houseFilePath, const std::string &houseName);

    void loadAlgorithms(const std::string &algoPath);

    void unloadAlgorithms();

    public: std::unique_ptr<AbstractAlgorithm> createAlgorithmByName(std::string name);

    int calculateScore(Status status);

    void displayMap(int iterations) const;

    int generateOutputFile(std::string algoName);

    void generateSummary() const;

    void clearCache();
};
