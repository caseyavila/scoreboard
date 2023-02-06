#pragma once
#include "visual.h"

struct cell {
    int r, g, b;
    int age;
};

class Waves : public Visual {

public:
    Waves();
    void Calculate(); // override
    void End(); // override
    cell* GetCell(int x, int y);
    void Propagate(cell* c1, cell* c2);
    void Age(cell* c);
    void DrawPoints();

protected:
    int mW, mH;
    cell* mBoard;
    float mPropFac, mLifetime, mFadeFac;
};

