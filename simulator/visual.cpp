#include "renderer.h"
#include "visual.h"
#include <iostream>
#include <SDL2/SDL.h>

Visual::Visual()
    :mRunning(true)
    ,mBoardSize((SDL_Point){64, 32})
{
}

bool Visual::Initialize() {
    // create renderer
    mRenderer = new Renderer(mBoardSize);
    if (!mRenderer->Create()) return false;
    // set previous time
    mPrevTime = SDL_GetTicks();
    return true;
}

void Visual::RunLoop() {
    while (mRunning){
        Input();
        Update();
        Calculate();
        Present();
    }
    End();
}

void Visual::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        mRunning = false;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void Visual::Update() {
    // Setting fps
    while ((int)SDL_GetTicks() < mPrevTime + 16) {}
    int currentTime = SDL_GetTicks();
    float deltaTime = (float)(currentTime - mPrevTime) / 1000;
    mPrevTime = currentTime;
    
    // clear screen
    mRenderer->ClearScreen();
}

void Visual::Calculate() {
}

void Visual::Present() {
    SDL_RenderPresent(mRenderer->GetSDLRenderer());
}

void Visual::End() {
    delete mRenderer;
    SDL_Quit();
}

