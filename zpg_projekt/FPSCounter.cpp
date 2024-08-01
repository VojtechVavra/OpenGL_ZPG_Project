#include "FPSCounter.hpp"
#include "MemoryUsage.hpp"


FPSCounter::FPSCounter()
{
}

void FPSCounter::enablePrint(bool enabled) {
    m_bPrintFps = enabled;
}

void FPSCounter::bindLambda(std::function<void(const int, std::string)> lambda) {
    storedLambda = lambda;
}


void FPSCounter::drawFps(double timepassed)
{
    /* Next we increment the number of frames that have passed;
       I'm assuming here that you're updating the framerate counter once per frame only */

    frames++;
    //And here we evaluate the actual FPS number.
    if (timepassed - startTime > frameRatePrint && frames > 10)
    {
        fps = (double)frames / (timepassed - startTime);
        startTime = timepassed;
        frames = 0;

        // casting: https://stackoverflow.com/questions/103512/why-use-static-casttx-instead-of-tx
        fps = static_cast<int>(fps);
        if (m_bPrintFps) {
            std::cout << fps << "\n";
        }
        executeLambda();
    }
}

void FPSCounter::executeLambda() const {
    if (storedLambda) {
        int memory_usage_in_mb = MemoryUsage::getMemoryUsage(); // This shows 80 MB but in Windows Task Manager it shows 60 MB
        // Maybe remove string with memory usage, not needed
        std::string memoryStr = " | " + std::to_string(memory_usage_in_mb) + " MB";
        storedLambda(fps, memoryStr);
    }
}
