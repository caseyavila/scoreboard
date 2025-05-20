#include "led-matrix.h"

#include "waves.h"

#include <unistd.h>
#include <csignal>
#include <iostream>

using namespace rgb_matrix;

volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
    interrupt_received = true;
}

static int usage(const char *progname) {
    fprintf(stderr, "usage: %s\n <directory path> <delay in microseconds>", progname);
    fprintf(stderr, "Display an animation of frames.\n");
    rgb_matrix::PrintMatrixFlags(stderr);
    return 1;
}

int main(int argc, char *argv[]) {
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;

    if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt)) {
        return usage(argv[0]);
    }

    RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);

    if (matrix == NULL) {
        return usage(argv[0]);
    }

    rgb_matrix::FrameCanvas *canvas = matrix->CreateFrameCanvas();
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    bool running = true;
    bool update = true;

    int mH = 32;
    int mW = 64;

    Waves* waves = new Waves(mW, mH);

    while (!interrupt_received && running) {
        if (update) {
            canvas->Clear();

            waves->Calculate();

            for (int i=0; i<mH; i++) {
                for (int j=0; j<mW; j++) {
                    cell* c = waves->GetCell(j, i);
                    //mRenderer->DrawPoint(j, i, c->r, c->g, c->b);
                    canvas->SetPixel(j, i, c->r, c->g, c->b);
                }
            }

            canvas = matrix->SwapOnVSync(canvas);
            usleep(std::stoi(argv[argc - 1]));
        }
    }

    // Finished. Shut down the RGB matrix.
    delete waves;
    delete matrix;
    printf("\n");
    return 0;
}
