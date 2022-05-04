#include "led-matrix.h"
#include "graphics.h"

#include "media.h"

#include <unistd.h>
#include <csignal>

using namespace rgb_matrix;

volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
    interrupt_received = true;
}

static int usage(const char *progname) {
    fprintf(stderr, "usage: <frame path>%s\n", progname);
    fprintf(stderr, "Display a frame.\n");
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

    std::vector<pixel> frame = parse_frame(argv[argc - 1]);

    while (!interrupt_received && running) {
        if (update) {
            canvas->Clear();
            for (pixel px : frame) {
                canvas->SetPixel(px.x, px.y, px.red, px.green, px.blue);
            }
            canvas = matrix->SwapOnVSync(canvas);
            update = false;  // Just render the frame once :)
        }
    }

    // Finished. Shut down the RGB matrix.
    delete matrix;
    printf("\n");
    return 0;
}
