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
    fprintf(stderr, "usage: %s\n <directory path> <delay in microseconds>", progname);
    fprintf(stderr, "Display an animation of frames.\n");
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
    bool update = true;

    std::vector<std::vector<pixel>> animation = parse_all_frames(argv[argc - 2]);
    unsigned int frame_index = 0;

    while (!interrupt_received && running) {
        if (update) {
            canvas->Clear();
            for (pixel px : animation[frame_index]) {
                canvas->SetPixel(px.x, px.y, px.red, px.green, px.blue);
            }
            frame_index = (frame_index + 1) % animation.size();
            canvas = matrix->SwapOnVSync(canvas);
            usleep(std::stoi(argv[argc - 1]));
        }
    }

    // Finished. Shut down the RGB matrix.
    delete matrix;
    printf("\n");
    return 0;
}
