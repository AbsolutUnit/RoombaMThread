//
// Created by Anshuman Funkwal on 3/31/23.
//

#include "../AlgorithmCommon/AlgorithmRegistration.h"
#include "include/Algo_112710107.h"
#include "cmath"

const Point origin = {0, 0};

double getEuclideanRadius(Point p) {
    return sqrt((p.x)^2 + (p.y)^2);
}

static bool compareEuclideanDistance(Point a, Point b) {
    return getEuclideanRadius(a) < getEuclideanRadius(b);
}


Step Algo_112710107::nextStep() {
    maxBattery = std::max(static_cast<int>(BMeter->getBatteryState()), maxBattery);
    // Initialize all Points in directions
    std::vector<Point> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    std::vector<Step> validSteps = {Step::North, Step::East, Step::South, Step::West};
    std::vector<Point> validPoints = {};

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
                if (dockMappings.count(facing) == 0) {

                    unvisited.push_back(facing);
                    Step oppositeStep = Algo_112710107::oppositeStep(static_cast<Step>(i));
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

    if (currentPosition == origin && (unvisited.size() == 0 || (currentPath.size() / 2 > maxSteps - stepCount) || maxSteps - stepCount < 2)) {
        // If so, return Finish
        return Step::Finish;
    }

    // Check for low battery or iterations almost complete
    int distanceToDock = dockMappings[currentPosition].size();
    if (distanceToDock == BMeter->getBatteryState() || maxSteps - stepCount <= distanceToDock + 1) {
        // Initialize copy of dock pathM
        currentPath = dockMappings[currentPosition];
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
    }

    if (currentPath.size() > 0) {
        return followPath(currentPath);
    }

    for (auto dir: validSteps) {
        validPoints.push_back(calculatePoint(currentPosition, dir));
    }

    std::sort(validPoints.begin(), validPoints.end(), compareEuclideanDistance);

    if (validPoints.size() > 0) {
        return calculateStep(currentPosition, validPoints[0]);
    }

    if (currentPosition == origin) {
        if (BMeter->getBatteryState() < currentPath.size() * 2) {
            ++stepCount;
            return Step::Stay;
        }

        Point next = unvisited.front();
        currentPath = reversePath(dockMappings[next]);
        unvisited.pop_front();

        if (currentPath.size() > (maxBattery / 2)) {
            currentPath.clear();
            unvisited.pop_front();
            ++stepCount;
            return Step::Stay;
        }

        return followPath(currentPath);
    } else {
        currentPath = dockMappings[currentPosition];
        return followPath(currentPath);
    }

    // Go North, then go East if last move is North, then go South if last move is East, go West if last move South
    // Of course don't make a move if it is visited, so if for example east is visited, go south again
    // If there is a wall, just choose the only valid (theoretically should be max 2, usually 1)
    // Once dead end reached, go to nextDest value (remember to trim nextDest as you traverse)
}

Point Algo_112710107::calculatePoint(Point x, Step y) const {
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

Step Algo_112710107::calculateStep(Point x, Point y) {
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

Step Algo_112710107::oppositeStep(Step s) const {
    // Initialize list of reversed directions
    Step reversed[] = {Step::South, Step::West, Step::North, Step::East};
    // Return the reverse of what was passed in
    return reversed[static_cast<int>(s)];
}

Step Algo_112710107::followPath(std::deque<Step>& d) {
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

std::deque<Step> Algo_112710107::reversePath(std::deque<Step>& path) const {
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



REGISTER_ALGORITHM(Algo_112710107);