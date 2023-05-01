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
    std::vector<Step> validSteps = {Step::North, Step::East, Step::South, Step::West};
    std::vector<Point> validPoints = {};

    // Loop through Points in directions
    for (int i = 0; i < 4; ++i) {
        // First cast to direction to check wall
        if (WSensor->isWall(static_cast<Direction>(i))) {
            // If it is a wall, skip
            std::vector<Step>::iterator pos = std::find(validSteps.begin(), validSteps.end(), static_cast<Step>(i));
            if (pos != validSteps.end()) {
                validSteps.erase(pos);
            }
            continue;
        } else {
            // Otherwise, initialize point facing
            Point facing = currentPosition + directions[i];
            // If node not in queue
            if (std::find(visited.begin(), visited.end(), facing) == visited.end()) {
                // Add to queue
                // Calculate opposite step
                if (std::find(unvisited.begin(), unvisited.end(), facing) == unvisited.end()) {
                    unvisited.push_back(facing);
                }
                if (dockMappings.count(facing) == 0) {
                    Step oppositeStep = Algo_112628028::oppositeStep(static_cast<Step>(i));
                    // Get mapping from current position
                    std::deque<Step> pathVector = dockMappings[currentPosition];
                    // Append opposing move to spot to dockMapping
                    pathVector.push_front(oppositeStep);
                    // Add to dock mapping
                    dockMappings[facing] = pathVector;
                }
            } else {
                std::vector<Step>::iterator pos = std::find(validSteps.begin(), validSteps.end(), static_cast<Step>(i));
                if (pos != validSteps.end()) {
                    validSteps.erase(pos);
                }
            }
        }
    }

    if (currentPosition == origin && (unvisited.size() == 0 || (currentPath.size() / 2 > maxSteps - stepCount))) {
            // If so, return Finish
            return Step::Finish;
    }

    // Check for low battery or iterations almost complete
    int distanceToDock = dockMappings[currentPosition].size();
    if (distanceToDock == BMeter->getBatteryState() || maxSteps - stepCount <= distanceToDock + 1) {
        // Initialize copy of dock path
        currentPath = dockMappings[currentPosition];
        if (std::find(visited.begin(), visited.end(), currentPosition) == visited.end()) {
            returnPos = currentPosition;
        } else {
            if (std::find(validSteps.begin(), validSteps.end(), prevMove) != validSteps.end()) {
                returnPos = calculatePoint(currentPosition, prevMove);
            } else if (std::find(unvisited.begin(), unvisited.end(), returnPos) == unvisited.end() && unvisited.size() > 0) {
                returnPos = unvisited.front();
                unvisited.pop_front();
            }
        }
        // Follow path, copy made since followPath pops
        return followPath(currentPath);
    }

    // If current location has dirt
    if (DSensor->dirtLevel() > 1) {
        // Clean dirt
        ++stepCount;
        return Step::Stay;
    } else {
        // Otherwise, remove current location from queue as it has been visited/cleaned
        std::deque<Point>::iterator pos = std::find(unvisited.begin(), unvisited.end(), currentPosition);
        if (pos != unvisited.end()) {
            unvisited.erase(pos);
            visited.push_back(currentPosition);
        }
        std::vector<Step>::iterator stepPos = std::find(validSteps.begin(), validSteps.end(), prevMove);
        if (stepPos != validSteps.end()) {
            validSteps.erase(stepPos);
        }
    }

    // If there is a next unvisited node path 
    if (currentPath.size() != 0) {
        // Follow that path
        return followPath(currentPath);
    }

    if (validSteps.size() != 0) {
        if (static_cast<int>(prevMove) % 2 == 0 && prevMove != Step::Stay) {
            std::vector<Step>::iterator pos = std::find(validSteps.begin(), validSteps.end(), prevMove);
            if (pos != validSteps.end()) {
                currentPosition = currentPosition + directions[static_cast<int>(prevMove)];
                return prevMove;
            } else {
                std::vector<Step>::iterator nPos = std::find(validSteps.begin(), validSteps.end(), Step::North);
                std::vector<Step>::iterator sPos = std::find(validSteps.begin(), validSteps.end(), Step::South);
                if (nPos != validSteps.end()) {
                    prevMove = Step::North;
                    currentPosition = currentPosition + directions[static_cast<int>(prevMove)];
                    return Step::North;
                } else if (sPos != validSteps.end()) {
                    prevMove = Step::South;
                    currentPosition = currentPosition + directions[static_cast<int>(prevMove)];
                    return Step::South;
                } else {
                    currentPosition = currentPosition + directions[static_cast<int>(validSteps[0])];
                    return validSteps[0];
                }
            }
        } else {
            std::vector<Step>::iterator nPos = std::find(validSteps.begin(), validSteps.end(), Step::North);
            std::vector<Step>::iterator sPos = std::find(validSteps.begin(), validSteps.end(), Step::South);
            if (nPos != validSteps.end()) {
                prevMove = Step::North;
                currentPosition = currentPosition + directions[static_cast<int>(prevMove)];
                return Step::North;
            } else if (sPos != validSteps.end()) {
                prevMove = Step::South;
                currentPosition = currentPosition + directions[static_cast<int>(prevMove)];
                return Step::South;
            } else {
                currentPosition = currentPosition + directions[static_cast<int>(validSteps[0])];
                return validSteps[0];
            }
        }
    }

    // If on the origin
    if (currentPosition == origin) {
        // Check if not charged
        if (BMeter->getBatteryState() < currentPath.size() * 2) {
            // Decide to charge
            ++stepCount;
            // Keep charging
            return Step::Stay;
        }

        if (unvisited.size() == 0 && returnPos == origin) {
            return Step::Finish;
        } else if (returnPos == origin) {
            Point nextStop = unvisited.front();
            std::deque<Step> nextStopMapping = dockMappings[nextStop];
            // Reverse the path to navigate there
            currentPath = reversePath(nextStopMapping);
            // Check if path to next stop is viable
            if (currentPath.size() > (maxBattery / 2)) {
                // If it is too long, clear it
                currentPath.clear();
                // Remove node from queue as it is unreachable
                unvisited.pop_front();
                // Stay
                ++stepCount;
                return Step::Stay;
            }

            return followPath(currentPath);
        } else {
            std::deque<Step> nextStopMapping = dockMappings[returnPos];
            // Reverse the path to navigate there
            currentPath = reversePath(nextStopMapping);
            // Check if path to next stop is viable
            if (currentPath.size() > (maxBattery / 2)) {
                // If it is too long, clear it
                currentPath.clear();
                // Remove node from queue as it is unreachable
                returnPos = origin;
                // Stay
                ++stepCount;
                return Step::Stay;
            }
            return followPath(currentPath);

        }

        // Call followPath once again
    } else {
        // If not on origin
        std::deque<Step> pathToDock = dockMappings[currentPosition];
        // Path back to origin
        return followPath(pathToDock);
    }
}

Point Algo_112628028::calculatePoint(Point x, Step y) const {
    int move = static_cast<int>(y);
    switch (move)
    {
    case 0:
        return Point{x.x, x.y - 1};
        break;

    case 1:
        return Point{x.x + 1, x.y};
        break;
    
    case 2:
        return Point{x.x, x.y + 1};
        break;

    case 3:
        return Point{x.x - 1, x.y};
        break;
    default:
        return x;
        break;
    }
}

Step Algo_112628028::calculateStep(Point x, Point y) {
    int dx = y.x - x.x;
    int dy = y.y - x.y;
    std::vector<Point> dir = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    std::vector<Point>::iterator pos = std::find(dir.begin(), dir.end(), Point{dx, dy});
    if (pos == dir.end()) {
        return Step::Stay;
    } else {
        currentPosition = currentPosition + dir[pos - dir.begin()];
        return static_cast<Step>(pos - dir.begin());
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
    if (static_cast<int>(nextStep) % 2 == 0) {
        prevMove = nextStep;
    }
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