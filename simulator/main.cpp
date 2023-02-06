#include "visual.h"
#include "waves.h"
#include <iostream>

int main(int argc, char* args[]) {
    Visual* visual = new Waves();
    
    if (visual->Initialize()) visual->RunLoop();
    else std::cout << "Failed to initialize";
    // End() called
    delete visual;

    return 0;
}

