#pragma once
#include <vector>
#include "BoxelGrid.h"

namespace almond {

    class BlobSystem2D {
    public:
        BlobSystem2D(BoxelGrid& grid);

        void update();
        void addBlob(float x, float y);

    private:
        struct Blob {
            float x, y;
        };

        std::vector<Blob> blobs;
        BoxelGrid& grid;
        const float blobSpeed = 1.0f;

        bool canMoveTo(float x, float y) const;
    };

}  // namespace almond
