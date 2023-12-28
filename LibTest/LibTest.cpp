#include "pch.h"
#include "CppUnitTest.h"
#include "..\hwVideoDecorder\VideoDecoder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LibTest
{
    TEST_CLASS(LibTest)
    {
    public:

        TEST_METHOD(InitializationTest)
        {
            // Arrange
            std::string videoPath = "C:\\path\\to\\your\\test\\video.mp4";  // Change this to your test video path
            VideoDecoder decoder(videoPath);

            // Act
            decoder.Initialize();

            // Assert
            // You can add assertions to check if initialization was successful
            // For example, check if decoder.window, decoder.renderer, etc. are not null
        }

        TEST_METHOD(PlayTest)
        {
            // Arrange
            std::string videoPath = "C:\\path\\to\\your\\test\\video.mp4";  // Change this to your test video path
            VideoDecoder decoder(videoPath);

            // Act
            decoder.Initialize();
            decoder.Play();

            // Assert
            // You can add assertions to check if Play() executes without exceptions
            Assert::IsTrue(true);  // Placeholder assertion
        }

        // Add more test methods for other functions in VideoDecoder class
    };
}
