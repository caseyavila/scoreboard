#include "led-matrix.h"
#include "graphics.h"

#include "media.h"

#include <unistd.h>
#include <csignal>
#include <iostream>

using namespace rgb_matrix;

enum Sides {
    red_gold,
    gold_red
};

volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
    interrupt_received = true;

    exit(EXIT_FAILURE);
}

static int usage(const char *progname) {
    fprintf(stderr, "usage: %s\n", progname);
    fprintf(stderr, "Display dye scoreboard.\n");
    rgb_matrix::PrintMatrixFlags(stderr);
    return 1;
}

void PlayAnimation(std::vector<std::vector<pixel>> animation, rgb_matrix::FrameCanvas* canvas,
        RGBMatrix* matrix, int microseconds) {
    for (std::vector<pixel> frame : animation) {
        canvas->Clear();
        for (pixel px : frame) {
            canvas->SetPixel(px.x, px.y, px.red, px.green, px.blue);
        }
        canvas = matrix->SwapOnVSync(canvas);
        usleep(microseconds);
    }
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

    std::vector<std::vector<pixel>> water_frames = parse_animation("../frames/water/");
    std::vector<std::vector<pixel>> number_frames = parse_animation("../frames/numbers/");
    std::vector<std::vector<pixel>> switch_rg_frames = parse_animation("../frames/switch-rg/");
    std::vector<std::vector<pixel>> switch_gr_frames = parse_animation("../frames/switch-gr/");
    std::vector<pixel> layout_frame = parse_frame("../frames/layout");


    int red_score = 0;
    int gold_score = 0;
    Sides sides = red_gold;

    std::string input;

    while (!interrupt_received && running) {
        canvas->Clear();
        // Set middle red and gold line
        for (pixel px : layout_frame) {
            canvas->SetPixel(px.x, px.y, px.red, px.green, px.blue);
        }

        // Offsets for positioning
        int leftx = 13;
        int rightx = 45;
        int y = 13;

        if (sides == red_gold) {
            // Make room for double digit numbers
            if (red_score > 9) leftx -= 4;
            if (gold_score > 9) rightx -= 4;

            // Set numbers
            for (pixel px : number_frames[red_score]) {
                canvas->SetPixel(px.x + leftx, px.y + y, 255, 0, 0);
            }
            for (pixel px : number_frames[gold_score]) {
                canvas->SetPixel(px.x + rightx, px.y + y, 255, 193, 7);
            }
        } else {
            // Make room for double digit numbers
            if (red_score > 9) rightx -= 4;
            if (gold_score > 9) leftx -= 4;

            // Set numbers
            for (pixel px : number_frames[red_score]) {
                canvas->SetPixel(px.x + rightx, px.y + y, 255, 0, 0);
            }
            for (pixel px : number_frames[gold_score]) {
                canvas->SetPixel(px.x + leftx, px.y + y, 255, 193, 7);
            }
        }

        // Sync leds with buffer
        canvas = matrix->SwapOnVSync(canvas);

        // Get new input
        if (std::getline(std::cin, input)) {
            if (input == "reset" && (red_score != 0 || gold_score != 0)) {
                red_score = 0;
                gold_score = 0;
                PlayAnimation(water_frames, canvas, matrix, 100000);
            } else if (input == "switch") {
                if (sides == red_gold) {
                    sides = gold_red;
                    PlayAnimation(switch_rg_frames, canvas, matrix, 100000);
                } else {
                    sides = red_gold;
                    PlayAnimation(switch_gr_frames, canvas, matrix, 100000);
                }
            } else if (input == "radd") {
                if (red_score < 12) {
                    red_score++;
                }
            } else if (input == "gadd") {
                if (gold_score < 12) {
                    gold_score++;
                }
            } else if (input == "rsub") {
                if (red_score > 0) {
                    red_score--;
                }
            } else if (input == "gsub") {
                if (gold_score > 0) {
                    gold_score--;
                }
            }
        } else {
            delete matrix;
            printf("\n");
            return 0;
        }
    }

    // Finished. Shut down the RGB matrix.
    delete matrix;
    printf("\n");
    return 0;
}
