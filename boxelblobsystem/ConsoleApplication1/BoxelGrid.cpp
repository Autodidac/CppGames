#include "BoxelGrid.h"
#include <iostream>

namespace almond {

    BoxelGrid::BoxelGrid(int width, int height)
        : width(width), height(height), grid(width* height, BoxelType::Air) {}

    BoxelType BoxelGrid::getBoxel(int x, int y) const {
        if (isValidPosition(x, y)) {
            return grid[y * width + x];
        }
        return BoxelType::Air;
    }

    void BoxelGrid::setBoxel(int x, int y, BoxelType type) {
        if (isValidPosition(x, y)) {
            grid[y * width + x] = type;
        }
    }

    void BoxelGrid::render() const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                std::cout << boxelChar(getBoxel(x, y));
            }
            std::cout << "\n";
        }
    }

    bool BoxelGrid::isValidPosition(int x, int y) const {
        return x >= 0 && y >= 0 && x < width && y < height;
    }

    char BoxelGrid::boxelChar(BoxelType type) const {
        switch (type) {
        case BoxelType::Air: return ' ';
        case BoxelType::Dirt: return '#';
        case BoxelType::Stone: return 'S';
        case BoxelType::Water: return '~';
        default: return '?';
        }
    }

}  // namespace almond
