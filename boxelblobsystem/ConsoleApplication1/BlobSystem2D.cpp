#include "BlobSystem2D.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace almond {

    BlobSystem2D::BlobSystem2D(BoxelGrid& grid) : grid(grid) {}

    void BlobSystem2D::update() {
        std::vector<Blob> newBlobs;

        for (auto& blob : blobs) {
            int x = static_cast<int>(blob.x);
            int y = static_cast<int>(blob.y);

            // Clear previous blob position
            grid.setBoxel(x, y, BoxelType::Air);

            // Debug output to check current blob state
            std::cout << "Blob at (" << blob.x << ", " << blob.y << ") - Int: (" << x << ", " << y << ")\n";
            std::cout.flush();  // Ensure immediate output

            // Determine new position
            if (canMoveTo(blob.x, blob.y + blobSpeed)) {
                blob.y += blobSpeed;
            }
            else if (canMoveTo(blob.x - blobSpeed, blob.y + blobSpeed)) {
                blob.x -= blobSpeed;
                blob.y += blobSpeed;
            }
            else if (canMoveTo(blob.x + blobSpeed, blob.y + blobSpeed)) {
                blob.x += blobSpeed;
                blob.y += blobSpeed;
            }

            x = static_cast<int>(blob.x);
            y = static_cast<int>(blob.y);

            // Ensure the blob stays within bounds and is valid
            if (grid.isValidPosition(x, y)) {
                newBlobs.push_back({ blob.x, blob.y });
            }
        }

        // Update grid with new blob positions
        for (const auto& blob : newBlobs) {
            grid.setBoxel(static_cast<int>(blob.x), static_cast<int>(blob.y), BoxelType::Water);
        }

        // Replace old blobs with updated positions
        blobs = std::move(newBlobs);

        // Add sleep to visualize updates
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Sleep for 500 milliseconds
    }

    void BlobSystem2D::addBlob(float x, float y) {
        blobs.push_back({ x, y });
    }

    bool BlobSystem2D::canMoveTo(float x, float y) const {
        int ix = static_cast<int>(x);
        int iy = static_cast<int>(y);
        return grid.getBoxel(ix, iy) == BoxelType::Air;
    }

}  // namespace almond
