#pragma once

#include <vector>
#include "../Common/common_types.h"
#include "../Common/common_structs.h"

class House {
    int totalDirt = 0;
    std::size_t maxSteps = 0;
    int numRows, numCols = 0;
    Point position = {-1, -1};
    Point dockLocation = {-1, -1};
    std::vector<char> houseMap;
public:
    House(int totalDirt, int numRows, int numCols, Point position, Point dockLocation, std::vector<char> &houseMap): totalDirt(totalDirt), numRows(numRows), numCols(numCols), position(position), dockLocation(dockLocation), houseMap(houseMap) {};

    bool isWall(Direction d) {
        std::vector<Point> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

        // Calculate new position in the direction given using mapping
        Point newPosition = position + directions[static_cast<int>(d)];

        // If this value is in range
        if (newPosition.x >= 0 && newPosition.x < numCols){
            // Double range check for readability
            if (newPosition.y >= 0 && newPosition.y < numRows) {
                // Get the value and check if it is dirt or dock
                char value = houseMap[convertToCoordinates(newPosition)];
                // Return whether or not it is (dirt/dock) or (wall)
                return value == WALL;
            } else {
                // Otherwise out of bounds for y so must be wall
                return true;
            }
        } else {
            // Also out of bounds case, for x
            return true;
        }   
    };

    int dirtLevel() {
        int coordinate = convertToCoordinates(position);

        // Check for if the position has valid dirt level
        if (houseMap[coordinate] <= '9' && houseMap[coordinate] > '0') {
            // If so, return the value as an int
            return houseMap[coordinate] - '0';
        }
        // Otherwise assume wall or dock and return 0
        return 0;
    }

    Point getPosition() {
        return position;
    }

    bool isDocked() {
        return position == dockLocation;
    }

    int getTotalDirt() {
        return totalDirt;
    }

    int getMaxSteps() {
        return maxSteps;
    }

    void handleMove(Step s) {
        // Take the current location value and check it is between 0 and 9
        int location = convertToCoordinates(position);
        if (houseMap[location] > '0' && houseMap[location] <= '9') {
            // Subtract dirt from the map location and from the total dirt
            --houseMap[location];
            --totalDirt;
        }

        // Initialize possible moves in order N E S W Stay
        std::vector<Point> moves = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}, {0, 0}};
        // Update position based on move taken
        position = position + moves[static_cast<int>(s)];
    }

    int convertToCoordinates(Point p) const {
        // Converts x, y coordinates to 1D vector format
        return p.y * numCols + p.x;
    }

    void displayHouse(int x) {
        // Function to track robot movements
        std::cout << "House Iteration #" << x << std::endl;
        std::cout << "________________________________________" << std::endl;
        for (int y = 0; y < numRows; ++y) {
            for (int x = 0; x < numCols; ++x) {
                if (y == position.y && x == position.x) {
                    std::cout << "*";
                } else {
                    std::cout << houseMap[convertToCoordinates({x, y})];
                }
            }
            std::cout << std::endl;
        }
        std::cout << "________________________________________" << std::endl;
        std::cout << "Robot Position Within House: (" << position.x << ", " << position.y << ")" << std::endl;
    }

    ~House(){};
};