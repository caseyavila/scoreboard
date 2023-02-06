#pragma once
#include "renderer.h"
#include <SDL2/SDL.h>

class Visual {
    
public:
    Visual();
    bool Initialize();
    void RunLoop();
    void Input();
    void Update();
    virtual void Calculate(); // overrideable
    void Present();
    virtual void End(); // overrideable
    
protected:
    bool mRunning;
    SDL_Point mBoardSize;
    int mPrevTime;
    class Renderer* mRenderer;
};

