/* Taken from (Maximus Minimus):
* https://gamedev.stackexchange.com/questions/83159/simple-framerate-counter
*/

#pragma once

#include <iostream>
#include <functional>


class FPSCounter
{
private:
    float frameRatePrint;    // cetnost vypisu do konzole fpsek
    int frames;
    double starttime;
    bool first;
    float fps;
    bool m_bPrintFps = true;
    //The first thing we do is check if this is the first time we've passed through the counter and set some stuff up:
    
    std::function<void(const int)> storedLambda;
    
public:
    /*template<typename T>
    //void bindFunction(std::function<T(float)> const& f) {
    void bindFunction(const T& f) {
        func = f;
    }
    template<typename F> void bindLambda(F f) {
        func2 = f;
    }*/

public:
    FPSCounter()
    {
        frameRatePrint = 0.25f; // original 0.25f
        frames = 0;
        starttime = 0;  // startTime;
        first = true;
        fps = 0.0f;
    }

    void enablePrint(bool enabled) {
        m_bPrintFps = enabled;
    }

    void bindLambda(std::function<void(const int)> lambda) {
        storedLambda = lambda;
    }

    void drawFps(double timepassed)
    {
        if (first)
        {
            //frames = 0;
            //starttime = timepassed;
            //first = FALSE;
            //return;
        }
        //Next we increment the number of frames that have passed; I'm assuming here that you're updating the framerate counter once per frame only :

        frames++;
        //And here we evaluate the actual FPS number.

        if (timepassed - starttime > frameRatePrint && frames > 10)
        {
            fps = (double)frames / (timepassed - starttime);
            starttime = timepassed;
            frames = 0;

            // casting: https://stackoverflow.com/questions/103512/why-use-static-casttx-instead-of-tx
            fps = static_cast<int>(fps);
            if (m_bPrintFps) {
                std::cout << fps << "\n";
            }
            executeLambda();
        }
    }

    void executeLambda() const {
        if (storedLambda) {
            storedLambda(fps);
        }
    }
};
