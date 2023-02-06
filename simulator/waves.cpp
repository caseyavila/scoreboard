#include <cstdlib>
#include <ctime>
#include <iostream>
#include "renderer.h"
#include "waves.h"

Waves::Waves()
    :mSpawnOdds(0.02f)
    ,mPropFac(0.01f)
    ,mAgeLuminance(255.0f) // could be int
    ,mLifetime(50.0f)
    ,mFadeFac(0.95f)
{
    // init random
    std::srand(std::time(nullptr)); // use curr time as seed

    // base class constructor called by default
    // bc it has no arguments (Visual())
    // mBoardSize specified by base class
    mW = mBoardSize.x;
    mH = mBoardSize.y;
    mBoard = new cell[mW*mH]; // dynamic 2d array
    
    // initialize board
    cell c;
    c.r = 0; c.g = 0; c.b = 0; c.age = 1.0f;
    for (int i=0; i<mH; i++) {
        for (int j=0; j<mW; j++) {
            *(mBoard + i * mW + j) = c;
        }
    }

    // Init conditions
    // GetCell(16, 15)->r = 255;
    // GetCell(48, 15)->b = 255;
}

void Waves::Calculate() { // override
    // begin random cell 
    // random four digit 0 to 1
    bool spawn = ((std::rand() % 10000) / 10000.0f) <= mSpawnOdds;
    if (spawn) {
        // random position
        float randX = std::rand() % 64;
        float randY = std::rand() % 32;
        // set cell to random color
        GetCell(randX, randY)->r = std::rand() % 255;
        GetCell(randX, randY)->g = std::rand() % 255;
        GetCell(randX, randY)->b = std::rand() % 255;
    }

    // traverse array
    for (int i=0; i<mH; i++) {
        for (int j=0; j<mW; j++) {
            // get value
            cell* c = GetCell(j, i);
            cell* n;

            // propagate to neighbors
            // left
            if (j-1 >= 0) {
                n = GetCell(j-1, i);
                Propagate(c, n);
            }
            // right
            if (j < mW-1) {
                n = GetCell(j+1, i);
                Propagate(c, n);
            }
            // down 
            if (i > 0) {
                n = GetCell(j, i-1);
                Propagate(c, n);
            }
            // up 
            if (i < mH-1) {
                n = GetCell(j, i+1);
                Propagate(c, n);
            }

            // age
            Age(c);
        }
    }

    DrawPoints();
}

cell* Waves::GetCell(int x, int y) {
    return mBoard + y * mW + x;
}

// from c1 into c2
void Waves::Propagate(cell* c1, cell* c2) {
    c2->r = c1->r * mPropFac + c2->r;
    c2->g = c1->g * mPropFac + c2->g;
    c2->b = c1->b * mPropFac + c2->b;
    // clamp
    if (c2->r > 255) c2->r = 255;
    if (c2->g > 255) c2->g = 255;
    if (c2->b > 255) c2->b = 255;
    // wrap
    // c2->r = c2->r % 255;
    // c2->g = c2->g % 255;
    // c2->b = c2->b % 255;
}

void Waves::Age(cell* c) {
    // increase age
    if (c->r + c->g + c->b >= mAgeLuminance)
        c->age++;
    // reset age
    else if (c->r == 0 && c->g == 0 && c->b == 0)
        c->age = 0;
    // fade
    if (c->age > mLifetime) {
        c->r *= mFadeFac;
        c->g *= mFadeFac;
        c->b *= mFadeFac;
    }
}

void Waves::DrawPoints() {
    for (int i=0; i<mH; i++) {
        for (int j=0; j<mW; j++) {
            cell* c = GetCell(j, i);
            mRenderer->DrawPoint(j, i, c->r, c->g, c->b);
        }
    }
}

void Waves::End() { // override
    delete[] mBoard;
    delete mRenderer;
    SDL_Quit();
}

