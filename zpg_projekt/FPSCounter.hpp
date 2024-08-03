/* Taken from (Maximus Minimus):
* https://gamedev.stackexchange.com/questions/83159/simple-framerate-counter
*/

#pragma once

#include <iostream>
#include <functional>


class FPSCounter
{
private:
    float frameRatePrint = 0.25f;    // cetnost vypisu do konzole fpsek, default 0.25f
    int frames = 0;
    double startTime = 0;
    bool first = true;
    float fps = 0.0f;
    bool m_bPrintFps = true;
    //The first thing we do is check if this is the first time we've passed through the counter and set some stuff up:
    
    // lambda function to be executed
    std::function<void(int)> storedLambda;
    std::function<void(const int, std::string)> storedLambda2;
    
    
public:
    FPSCounter();
    void enablePrint(bool enabled);

    void bindLambda(std::function<void(const int)> lambda);
    void bindLambda2(std::function<void(const int, std::string)> lambda);

    void drawFps(double timepassed);

    void executeLambda() const;
};
