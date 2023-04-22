//
// Created by Anshuman Funkwal on 3/31/23.
//

#include "../AlgorithmCommon/AlgorithmRegistration.h"
#include "include/Algo_112628028.h"

const Point origin = {0, 0};

Step Algo_112628028::nextStep() {
    maxBattery = std::max(static_cast<int>(BMeter->getBatteryState()), maxBattery);
    // Initialize all Points in directions
    std::vector<Point> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    // Loop through Points in directions
    for (int i = 0; i < 4; ++i) {
        // First cast to direction to check wall
        if (WSensor->isWall(static_cast<Direction>(i))) {
            // If it is a wall, skip
            continue;
        } else {
            // Otherwise, initialize point facing
            Point facing = currentPosition + directions[i];
            // If node not in queue
            if (nextDest.size() == 0 || std::find(nextDest.begin(), nextDest.end(), facing) == nextDest.end()) {
                // Add to queue
                nextDest.push_back(facing);
                // Calculate opposite step
                if (dockMappings.count(facing) == 0) {
                    Step oppositeStep = Algo_112628028::oppositeStep(static_cast<Step>(i));
                    // Get mapping from current position
                    std::deque<Step> pathVector = dockMappings[currentPosition];
                    // Append opposing move to spot to dockMapping
                    pathVector.push_front(oppositeStep);
                    // Add to dock mapping
                    dockMappings[facing] = pathVector;
                }
            }
        }
    }

    if (currentPosition == origin && (nextDest.size() == 0 || maxSteps - stepCount <= 2)) {
            // If so, return Finish
            return Step::Finish;
    }

    // Check for low battery or iterations almost complete
    int distanceToDock = dockMappings[currentPosition].size();
    if (distanceToDock == BMeter->getBatteryState() + 1 || maxSteps - stepCount <= distanceToDock + 2) {
        // Initialize copy of dock pathM
        std::deque<Step> pathToDock = dockMappings[currentPosition];
        // Follow path, copy made since followPath pops
        return followPath(pathToDock);
    }

    // If current location has dirt
    if (DSensor->dirtLevel() > 1) {
        // Clean dirt
        ++stepCount;
        return Step::Stay;
    } else {
        // Otherwise, remove current location from queue as it has been visited/cleaned
        if (currentPosition == nextDest.front()) {
            nextDest.pop_front();
        }
    }

    // If there is a next unvisited node path 
    if (currentPath.size() != 0) {
        // Follow that path
        return followPath(currentPath);
    }

    // If on the origin
    if (currentPosition == origin) {
        // Check if not charged
        if (BMeter->getBatteryState() != maxBattery) {
            // Decide to charge
            ++stepCount;
            // Keep charging
            return Step::Stay;
        }

        // Initialize a new path by first taking queued first element
        Point nextStop = nextDest.front();

        // Initialize path from dock of that queued element
        std::deque<Step> nextStopMapping = dockMappings[nextStop];
        // Reverse the path to navigate there
        currentPath = reversePath(nextStopMapping);
        // Check if path to next stop is viable
        if (currentPath.size() > (maxBattery / 2)) {
            // If it is too long, clear it
            currentPath.clear();
            // Remove node from queue as it is unreachable
            nextDest.pop_front();
            // Stay
            ++stepCount;
            return Step::Stay;
        }

        // Call followPath once again
        return followPath(currentPath);
    } else {
        // If not on origin
        std::deque<Step> pathToDock = dockMappings[currentPosition];
        // Path back to origin
        return followPath(pathToDock);
    }

}

Step Algo_112628028::oppositeStep(Step s) const {
    // Initialize list of reversed directions
    Step reversed[] = {Step::South, Step::West, Step::North, Step::East};
    // Return the reverse of what was passed in
    return reversed[static_cast<int>(s)];
}

Step Algo_112628028::followPath(std::deque<Step>& d) {
    // Take the front step
    if (d.size() == 0) {
        return Step::Stay;
    }

    Step nextStep = d.front();
    // Pop it from the path
    d.pop_front();

    std::vector<Point> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    currentPosition = currentPosition + directions[static_cast<int>(nextStep)];

    // Add a step since algorithm is making a move
    ++stepCount;
    // Return the move to make
    return nextStep;
}

std::deque<Step> Algo_112628028::reversePath(std::deque<Step>& path) const {
    // Initialize a deque for flipped path
    std::deque<Step> flippedPath;

    // Loop through in reverse
    for (auto itx = path.rbegin(); itx != path.rend(); ++itx) {
        // Push back the reverse of the step in reverse
        flippedPath.push_back(oppositeStep(*itx));
    }

    // Return the flipped path
    return flippedPath;
}


REGISTER_ALGORITHM(Algo_112628028);