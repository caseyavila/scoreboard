#include "led-matrix.h"
#include "graphics.h"

#include <unistd.h>

#include <csignal>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

struct pixel {
    int x;
    int y;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

using namespace rgb_matrix;

volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
    interrupt_received = true;
}

static int usage(const char *progname) {
    fprintf(stderr, "usage: %s\n", progname);
    fprintf(stderr, "Display dye scoreboard.\n");
    rgb_matrix::PrintMatrixFlags(stderr);
    return 1;
}

static std::vector<pixel> parse_frame(std::string filename) {
    std::vector<pixel> pixels;

    std::ifstream frame_stream(filename);
    std::string line;

    while (std::getline(frame_stream, line)) {
        std::istringstream iline(line);
        std::string item;
        struct pixel px;
        for (int i = 0; std::getline(iline, item, ' '); i++) {
            switch (i) {
                case 0:
                    px.x = stoi(item);
                    break;
                case 1:
                    px.y = stoi(item);
                    break;
                case 2:
                    px.red = stoi(item);
                    break;
                case 3:
                    px.green = stoi(item);
                    break;
                case 4:
                    px.blue = stoi(item);
                    break;
                default:
                    break;
            }
        }
        pixels.push_back(px);
    }

    return pixels;
}

static std::vector<std::vector<pixel>> parse_all_frames(std::string directory) {
    std::vector<std::vector<pixel>> all_frames;

    for (int i = 0; std::filesystem::exists(directory + "frame" + std::to_string(i)); i++) {
        all_frames.push_back(parse_frame(directory + "frame" + std::to_string(i)));
    }

    return all_frames;
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

    std::vector<std::vector<pixel>> water_frames = parse_all_frames("../frames/water/");
    std::vector<std::vector<pixel>> number_frames = parse_all_frames("../frames/numbers/");
    std::vector<pixel> layout_frame = parse_frame("../frames/layout");

    int red_score = 0;
    int gold_score = 0;

    std::string input;

    while (!interrupt_received && running) {
        canvas->Clear();
        // Set middle red and gold line
        for (pixel px : layout_frame) {
            canvas->SetPixel(px.x, px.y, px.red, px.green, px.blue);
        }

        // Offsets for positioning
        int redx = 13;
        int goldx = 45;
        int y = 13;

        // Make room for double digit numbers
        if (red_score > 9) redx -= 4;
        if (gold_score > 9) goldx -= 4;

        // Set numbers
        for (pixel px : number_frames[red_score]) {
            canvas->SetPixel(px.x + redx, px.y + y, 255, 0, 0);
        }
        for (pixel px : number_frames[gold_score]) {
            canvas->SetPixel(px.x + goldx, px.y + y, 255, 193, 7);
        }

        // Sync leds with buffer
        canvas = matrix->SwapOnVSync(canvas);

        // Get new input
        if (std::getline(std::cin, input)) {
            if (input == "reset") {
                red_score = 0;
                gold_score = 0;
            } else if (input == "radd") {
                red_score++;
            } else if (input == "gadd") {
                gold_score++;
            } else if (input == "rsub") {
                red_score--;
            } else if (input == "gsub") {
                gold_score--;
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
