// PongGame.h
#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h> // For _kbhit() and _getch()

class PongGame {
public:
    PongGame();
    void run();

private:
    int ballX, ballY;
    int paddle1Y, paddle2Y;
    int ballDirX, ballDirY;
    const int width, height;

    void draw();
    void input();
    void logic();
};

inline PongGame::PongGame()
    : ballX(5), ballY(3), paddle1Y(2), paddle2Y(2), ballDirX(1), ballDirY(1), width(10), height(5) {}

inline void PongGame::draw() {
    system("cls"); // Clear the console

    // Draw the game area
    for (int i = 0; i < height + 2; ++i) {
        for (int j = 0; j < width + 2; ++j) {
            if (j == 0 || j == width + 1) {
                std::cout << "#"; // Wall
            }
            else if (i == 0 || i == height + 1) {
                std::cout << "#"; // Wall
            }
            else if (j == 1 && i >= paddle1Y && i < paddle1Y + 3) {
                std::cout << "|"; // Left paddle
            }
            else if (j == width && i >= paddle2Y && i < paddle2Y + 3) {
                std::cout << "|"; // Right paddle
            }
            else if (j == ballX && i == ballY) {
                std::cout << "O"; // Ball
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

inline void PongGame::input() {
    if (_kbhit()) {
        char current = _getch();
        if (current == 'w' && paddle1Y > 1) {
            paddle1Y--; // Move left paddle up
        }
        else if (current == 's' && paddle1Y < height - 2) {
            paddle1Y++; // Move left paddle down
        }
        else if (current == 'i' && paddle2Y > 1) {
            paddle2Y--; // Move right paddle up
        }
        else if (current == 'k' && paddle2Y < height - 2) {
            paddle2Y++; // Move right paddle down
        }
    }
}

inline void PongGame::logic() {
    // Move the ball
    ballX += ballDirX;
    ballY += ballDirY;

    // Check for collision with top or bottom
    if (ballY <= 1 || ballY >= height) {
        ballDirY = -ballDirY; // Reverse direction
    }

    // Check for collision with paddles
    if (ballX == 1 && ballY >= paddle1Y && ballY < paddle1Y + 3) {
        ballDirX = -ballDirX; // Reverse direction
    }
    if (ballX == width && ballY >= paddle2Y && ballY < paddle2Y + 3) {
        ballDirX = -ballDirX; // Reverse direction
    }

    // Reset ball if it goes out of bounds
    if (ballX <= 0 || ballX >= width + 1) {
        ballX = width / 2; // Reset position
        ballY = height / 2;
    }
}

inline void PongGame::run() {
    while (true) {
        draw();
        input();
        logic();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Control the frame rate
    }
}
