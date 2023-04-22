//
// Created by Anshuman Funkwal on 3/31/23.
//

#ifndef A2_ALGO_112628028_H
#define A2_ALGO_112628028_H

#include <cstddef>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include "../../Common/AbstractAlgorithm.h"
#include "../../Common/common_types.h"
#include "../../Common/common_structs.h"

class Algo_112628028: public AbstractAlgorithm {
public:
    int maxBattery = 0;
    std::size_t stepCount = 0;
    std::size_t maxSteps = 0;
    const WallsSensor *WSensor = nullptr;
    const DirtSensor *DSensor = nullptr;
    const BatteryMeter *BMeter = nullptr;
    std::deque<Step> pathToDock;
    std::deque<Step> currentPath;
    std::unordered_map<Point, std::deque<Step>, PointHash> dockMappings = {
        {{0, 0}, {}}
    };
    Point currentPosition = {0, 0};
    std::deque<Point> nextDest;
    

public:

    Algo_112628028(){};

    void setMaxSteps(std::size_t maxSteps) override {
        this->maxSteps = maxSteps;
    }
    void setWallsSensor(const WallsSensor& wallsSensor) override {
        this->WSensor = &wallsSensor;
    }
    void setDirtSensor(const DirtSensor& dirtSensor)  override {
        this->DSensor = &dirtSensor;
    }
    void setBatteryMeter(const BatteryMeter& batteryMeter) override {
        this->BMeter = &batteryMeter;
    }
    // TODO : Complete in cpp file
    Step nextStep() override;

    ~Algo_112628028(){};
    
private:
    Step oppositeStep(Step s) const;
    Step followPath(std::deque<Step>& d);
    std::deque<Step> reversePath(std::deque<Step>& path) const;
};

#endif //A2_ALGO_112628028_H
