#undef main
// ConsoleApp.cpp
#include "..\hwVideoDecorder\VideoDecoder.h"
#include <exception>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        VideoDecoder videoDecoder("C:\\Users\\TUMUTECH\\Downloads\\Video\\a.mp4");
        videoDecoder.Initialize();
        videoDecoder.Play();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}