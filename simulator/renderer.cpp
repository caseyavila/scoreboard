#include "renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "math.h"

Renderer::Renderer(SDL_Point boardSize)
    :mScaleFactor(16)

{
    // calculate window size from board size
    mWindowSize = (SDL_Point) {
        boardSize.x * mScaleFactor,
        boardSize.y * mScaleFactor
    };
}

Renderer::~Renderer() {
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mSDLRenderer);
}

bool Renderer::Create() {
    // init video
    int initialized = SDL_Init(SDL_INIT_VIDEO);
    if (initialized != 0) {
        return false;
    }
    // create window
    mWindow = SDL_CreateWindow("Scoreboard Sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowSize.x, mWindowSize.y, 0); // set fullscreen
    if (mWindow == NULL) {
        return false;
    }
    // create renderer
    mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (mSDLRenderer == NULL) {
        return false;
    }
    return true;
}

void Renderer::ClearScreen() {
    SDL_SetRenderDrawColor(mSDLRenderer, 0, 0, 0, 0);
    SDL_RenderClear(mSDLRenderer);
}

void Renderer::DrawPoint(int xPos, int yPos, int r, int g, int b) {
    SDL_SetRenderDrawColor(mSDLRenderer, r, g, b, 255);
    // draws box (one scoreboard pixel)
    SDL_Rect rect;
    rect.x = xPos * mScaleFactor;
    rect.y = yPos * mScaleFactor;
    rect.w = mScaleFactor;
    rect.h = mScaleFactor;
    SDL_RenderFillRect(mSDLRenderer, &rect);
}

