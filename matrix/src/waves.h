#pragma once

struct cell {
    int r, g, b;
    int age;
};

class Waves {

public:
    Waves(int mW, int mH);
    void Calculate(); // override
    void End(); // override
    cell* GetCell(int x, int y);
    void Propagate(cell* c1, cell* c2);
    void Age(cell* c);
    void DrawPoints();
    cell* GetBoard();

protected:
    int mW, mH;
    cell* mBoard;
    float mSpawnOdds, mPropFac, mAgeLuminance, mLifetime, mFadeFac;
};

