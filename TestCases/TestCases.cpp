#include "pch.h"
#include "CppUnitTest.h"
#include "..\hwVideoDecorder\VideoDecoder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestCases
{
    TEST_CLASS(TestCases)
    {
    public:

        TEST_METHOD(TestInitializeAndPlayVideo)
        {
            // Arrange
            // Initialize SDL before creating VideoDecoder
            if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                Assert::Fail(L"SDL initialization failed");
            }

            VideoDecoder videoDecoder("C:\\Users\\TUMUTECH\\Pictures\\Camera Roll\\c.mp4");

            // Act and Assert
            try
            {
                videoDecoder.Initialize();
                // Your other assertions or checks go here
            }
            catch (const std::exception& ex)
            {
                // Handle the exception or fail the test if needed
                Assert::Fail(L"Exception occurred: ");
            }
        }


        TEST_METHOD(TestInitializationErrorHandling)
        {
            // Arrange
            VideoDecoder videoDecoderInvalidPath("C:\\nonexistent\\path\\video.mp4");
            VideoDecoder videoDecoderUnsupportedCodec("C:\\path\\to\\unsupported\\codec.mp4");

            // Act and Assert
            try
            {
                videoDecoderInvalidPath.Initialize();
                // Fail the test if it doesn't throw an exception for an invalid path
                Assert::Fail(L"Expected exception for invalid path.");

            }
            catch (const std::exception&)
            {
                // Expected exception for invalid path
            }

            try
            {
                videoDecoderUnsupportedCodec.Initialize();
                // Fail the test if it doesn't throw an exception for an unsupported codec
                Assert::Fail(L"Expected exception for unsupported codec.");
            }
            catch (const std::exception&)
            {
                // Expected exception for unsupported codec
            }
        }

        TEST_METHOD(TestSDLInitializationFailure)
        {
            // Arrange
            VideoDecoder videoDecoder("C:\\Users\\TUMUTECH\\Pictures\\Camera Roll\\c.mp4");

            // Mocking SDL_Init failure by setting invalid flags
            SDL_setenv("SDL_VIDEO_X11_VISUALID", "invalid_flags", 1);

            // Act and Assert
            try
            {
                videoDecoder.Initialize();
                // Fail the test if it doesn't throw an exception for SDL initialization failure
                Assert::Fail(L"Expected exception for SDL initialization failure.");
            }
            catch (const std::exception&)
            {
                // Expected exception for SDL initialization failure
            }
        }

        TEST_METHOD(TestVideoDecodingPerformance)
        {
            // Arrange
            // Initialize SDL before creating VideoDecoder
            if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                Assert::Fail(L"SDL initialization failed");
            }

            VideoDecoder videoDecoder("C:\\Users\\TUMUTECH\\Pictures\\Camera Roll\\c.mp4");

            // Act and Assert
            try
            {
                videoDecoder.Initialize();
                // Your other performance-related assertions or checks go here
                videoDecoder.Play();  // Assuming Play is responsible for video decoding and rendering
            }
            catch (const std::exception& ex)
            {
                // Handle the exception or fail the test if needed
                Assert::Fail(L"Exception occurred: ");
            }
        }

    };
}
