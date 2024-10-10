#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>

// Simple 3D Vector class
struct Vec3 {
    float x, y, z;
};

// Platform detection macros
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Console manipulation class for cross-platform functionality
class CrossPlatformConsole {
public:
    CrossPlatformConsole(int width, int height) : width(width), height(height) {
#ifdef _WIN32
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#else
        // ANSI escape codes are used for non-Windows systems
#endif
        frameBuffer.resize(height, std::string(width, ' '));  // Initialize empty screen buffer
    }

    // Clears the console screen
    void clear(char fill = ' ') {
        for (auto& row : frameBuffer) {
            row.assign(width, fill);  // Fill each row with the character
        }

#ifdef _WIN32
        // Windows-specific console clearing
        COORD tl = { 0, 0 };
        CONSOLE_SCREEN_BUFFER_INFO s;
        GetConsoleScreenBufferInfo(hConsole, &s);
        DWORD written, cells = s.dwSize.X * s.dwSize.Y;
        FillConsoleOutputCharacter(hConsole, fill, cells, tl, &written);
        FillConsoleOutputAttribute(hConsole, s.wAttributes, cells, tl, &written);
        SetConsoleCursorPosition(hConsole, tl);
#else
        // ANSI escape code for clearing the screen
        std::cout << "\033[2J\033[H" << std::flush;
#endif
    }

    // Draw a line between two points in the buffer
    void drawLine(int x1, int y1, int x2, int y2, char ch) {
        int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        int err = dx + dy, e2;
        while (true) {
            if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
                frameBuffer[y1][x1] = ch;  // Draw pixel in buffer
            if (x1 == x2 && y1 == y2) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x1 += sx; }
            if (e2 <= dx) { err += dx; y1 += sy; }
        }
    }

    // Render the frame buffer to the console
    void render() {
#ifdef _WIN32
        COORD coord = { 0, 0 };  // Move cursor to top-left on Windows
        SetConsoleCursorPosition(hConsole, coord);
#else
        // ANSI escape code to move the cursor to top-left on non-Windows
        std::cout << "\033[H";
#endif

        for (const auto& row : frameBuffer) {
            std::cout << row << std::endl;
        }
    }

private:
    int width, height;
    std::vector<std::string> frameBuffer;
#ifdef _WIN32
    HANDLE hConsole;
#endif
};

// Projection function: Project 3D point into 2D
Vec3 project(const Vec3& point, float screenWidth, float screenHeight, float fov, float viewerDistance) {
    float factor = fov / (viewerDistance + point.z);
    return { point.x * factor + screenWidth / 2, point.y * factor + screenHeight / 2, 0 };
}

// Cube vertices
std::vector<Vec3> cubeVertices = {
    {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},  // Back face
    {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}       // Front face
};

// Cube edges (pairs of vertex indices)
std::vector<std::pair<int, int>> cubeEdges = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},  // Back face
    {4, 5}, {5, 6}, {6, 7}, {7, 4},  // Front face
    {0, 4}, {1, 5}, {2, 6}, {3, 7}   // Connecting edges
};

// Rotate point around the Y axis
Vec3 rotateY(const Vec3& point, float angle) {
    float s = std::sin(angle);
    float c = std::cos(angle);
    return { point.x * c + point.z * s, point.y, -point.x * s + point.z * c };
}

// Example usage: a rotating wireframe cube in a cross-platform console
int main() {
    CrossPlatformConsole renderer(80, 40);  // Create a console renderer for 80x40 resolution
    float angle = 0.0f;

    while (true) {
        renderer.clear();  // Clear the screen

        // Rotate and project vertices
        std::vector<Vec3> projectedVertices;
        for (const auto& v : cubeVertices) {
            Vec3 rotated = rotateY(v, angle);
            projectedVertices.push_back(project(rotated, 80, 40, 50.0f, 3.0f));
        }

        // Draw cube edges
        for (const auto& edge : cubeEdges) {
            int x1 = static_cast<int>(projectedVertices[edge.first].x);
            int y1 = static_cast<int>(projectedVertices[edge.first].y);
            int x2 = static_cast<int>(projectedVertices[edge.second].x);
            int y2 = static_cast<int>(projectedVertices[edge.second].y);
            renderer.drawLine(x1, y1, x2, y2, '#');
        }

        renderer.render();  // Render the frame buffer to the console

        angle += 0.05f;  // Rotate cube

        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate frame rate
    }

    return 0;
}
