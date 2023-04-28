#pragma once
#include <functional>

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return (x == other.x) && (y == other.y);
    }

    Point operator+(Point& other) const {
        return {x + other.x, y + other.y};
    }
};

struct PointHash {
    std::size_t operator()(const Point& p) const {
        std::size_t x_hash = std::hash<int>()(p.x);
        std::size_t y_hash = std::hash<int>()(p.y);
        return x_hash ^ (y_hash << 1);
    }
};

enum class Status {FINISHED, WORKING, DEAD};

// Character to represent the Dock
const char DOCK = 'D';

// Character to represent a Wall
const char WALL = 'W';