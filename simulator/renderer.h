#pragma once
#include "math.h"
#include <SDL2/SDL.h>

class Renderer {
    
public:
    Renderer(SDL_Point boardSize);
    ~Renderer();
    bool Create();
    void ClearScreen();
    void DrawPoint(int xPos, int yPos, int r, int g, int b);
    SDL_Renderer* GetSDLRenderer() { return mSDLRenderer; }
    // SDL_Point GetBoardSize() { return mBoardSize; }
    
protected:
    SDL_Point mWindowSize;
    SDL_Window* mWindow;
    SDL_Renderer* mSDLRenderer;
    int mScaleFactor = 8;
};

