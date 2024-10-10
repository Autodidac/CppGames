#include <SFML/Graphics.hpp>
#include "BlobSystem2D.h"
#include "BoxelGrid.h"

// Convert BoxelType to SFML color
sf::Color boxelColor(almond::BoxelType type) {
    switch (type) {
    case almond::BoxelType::Air: return sf::Color::White;
    case almond::BoxelType::Dirt: return sf::Color(139, 69, 19); // Brown
    case almond::BoxelType::Stone: return sf::Color(128, 128, 128); // Gray
    case almond::BoxelType::Water: return sf::Color(0, 0, 255); // Blue
    default: return sf::Color::Black;
    }
}

int main() {
    const int width = 20;
    const int height = 10;
    const int cellSize = 30;

    sf::RenderWindow window(sf::VideoMode(width * cellSize, height * cellSize), "Blob System");

    almond::BoxelGrid boxelGrid(width, height);

    // Fill the bottom row with stone
    for (int x = 0; x < width; ++x) {
        boxelGrid.setBoxel(x, height - 1, almond::BoxelType::Stone);
    }

    boxelGrid.setBoxel(10, 4, almond::BoxelType::Stone);

    // Create the blob system
    almond::BlobSystem2D blobSystem(boxelGrid);

    // Add some blobs (water)
    blobSystem.addBlob(10.0f, 0.0f);
    blobSystem.addBlob(10.0f, 1.0f);
    blobSystem.addBlob(10.0f, 2.0f);


    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        blobSystem.update();

        window.clear(sf::Color::Black);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                cell.setPosition(x * cellSize, y * cellSize);
                cell.setFillColor(boxelColor(boxelGrid.getBoxel(x, y)));
                window.draw(cell);
            }
        }

        window.display();
        sf::sleep(sf::milliseconds(100)); // Adjusted frame rate for better visibility
    }

    return 0;
}
