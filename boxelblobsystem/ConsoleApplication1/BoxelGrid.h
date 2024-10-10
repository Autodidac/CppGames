#pragma once
#include <vector>
#include <cstdint>

namespace almond {

    enum class BoxelType : uint8_t {
        Air = 0,
        Dirt,
        Stone,
        Water
    };

    class BoxelGrid {
    public:
        BoxelGrid(int width, int height);

        BoxelType getBoxel(int x, int y) const;
        void setBoxel(int x, int y, BoxelType type);
        void render() const;

        // Make isValidPosition public
        bool isValidPosition(int x, int y) const;

    private:
        int width, height;
        std::vector<BoxelType> grid;

        char boxelChar(BoxelType type) const;
    };

}  // namespace almond
