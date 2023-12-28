#undef main
// ConsoleApp.cpp
#include "..\hwVideoDecorder\VideoDecoder.h"
#include <exception>
#include <iostream>

int main(int argc, char* argv[]) {
    // Initialize SDL before creating VideoDecoder
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error: SDL initialization failed - " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    try {
        VideoDecoder videoDecoder("C:\\Users\\TUMUTECH\\Pictures\\Camera Roll\\c.mp4");
        videoDecoder.Initialize();
        videoDecoder.Play();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        SDL_Quit();  // Ensure SDL is properly cleaned up
        return EXIT_FAILURE;
    }

    SDL_Quit();  // Ensure SDL is properly cleaned up
    return EXIT_SUCCESS;
}
