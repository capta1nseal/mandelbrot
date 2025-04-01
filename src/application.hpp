#ifndef _MANDELBROTAPPLICATION
#define _MANDELBROTAPPLICATION

#include <chrono>
#include <thread>

#include <SDL3/SDL.h>

#include "shading.hpp"
#include "solver.hpp"

std::chrono::_V2::steady_clock::time_point now();

// Wrapper class for the application.
// Due to being lazy, holds the code for SDL3.
class MandelbrotApplication {
public:
    MandelbrotApplication();

    void run();

private:
    bool isRunning;
    int frameCounter;
    double animationTime;
    double animationSpeed;
    unsigned int displayWidth, displayHeight;
    bool isFullscreen;

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *renderTexture;
    unsigned char *texturePixels;
    int texturePitch;

    SDL_Event event;
    const bool *keyboardState;

    SDL_FPoint mousePosition;

    Solver solver;
    std::jthread solverThread;

    Shading shading;

    void initializeSdl();
    void destroySdl();

    void initializeGrid();

    void initializeShading();

    void initializeRenderTexture();

    void handleEvents();

    void draw();
};

#endif
