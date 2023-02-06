#include "renderer.h"
#include "waves.h"

Waves::Waves()
    :mPropFac(0.01)
    ,mLifetime(100)
    ,mFadeFac(0.95)
{
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

    // init conditions
    GetCell(16, 15)->r = 255;
    GetCell(48, 15)->b = 255;
}

void Waves::Calculate() { // override
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
    if (c2->r > 255) c2->r = 255;
    c2->g = c1->g * mPropFac + c2->g;
    if (c2->g > 255) c2->g = 255;
    c2->b = c1->b * mPropFac + c2->b;
    if (c2->b > 255) c2->b = 255;
}

void Waves::Age(cell* c) {
    if (c->r == 255 || c->g == 255 || c->b == 255)
        c->age++;

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

