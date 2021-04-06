#ifndef FLAME_HPP
#define FLAME_HPP

#include <GL/glew.h>

#include "TextureManager.hpp"

// abstract class
class Animation
{
public:
    // interface
    virtual void AnimateNextFrame(float timestep) = 0;
};

class Flame : public Animation
{
private:
    //float frameRatePrint;    // cetnost vypisu do konzole fpsek
    //int frames;
    //double starttime;
    //bool first;
    //float fps;
    //The first thing we do is check if this is the first time we've passed through the counter and set some stuff up:

public:
    int index;

    virtual void AnimateNextFrame(float timestep)	// in ms
    {
        static float totaltimepassed = 0;
        totaltimepassed += timestep;

        index = ((int)(totaltimepassed / m_msperpicture)) % 8;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture[index]->getTextureId());
        
        // render quad ... etc ...
    }

    Flame()
    {
        auto textureManager = TextureManager::getInstance();

        for (unsigned int i = 0; i < 8; i++) {
            m_texture[i] = textureManager->getModelTexture("textures\\flame\\flame" + std::to_string(i + 1) + ".tga");
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texture[i]->getTextureId());
        }
        

        /*frameRatePrint = 0.25f; // original 0.25f
        frames = 0;
        starttime = 0;  // startTime;
        first = true;
        fps = 0.0f;*/
    }
private:
    std::shared_ptr<Texture> m_texture[10];
    float m_msperpicture{ 50.0f };   // in ms, original 500

    /*void redraw(double timepassed)
    {
        frames++;
        //And here we evaluate the actual FPS number.

        if (timepassed - starttime > frameRatePrint && frames > 10)
        {
            glBindTexture(GL_TEXTURE_2D, textures[timePassed % 10]);
        }
    }*/
};

#endif      // !FLAME_HPP