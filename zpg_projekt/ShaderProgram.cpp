#include "ShaderProgram.hpp"
#include "Camera.hpp"

ShaderProgram::ShaderProgram(ShaderType shaderType, GLuint shaderProgram)
{
    this->shaderType = shaderType;
    this->shaderProgram = shaderProgram;
}

ShaderProgram::ShaderProgram() {}

void ShaderProgram::update(Camera* camera, camChange cameraChange)
{
    Shader::use(shaderProgram);

    if (cameraChange == camChange::MOVE_ROTATE) {
        // nepridavat vypisy/printy sekala by se scena
        if (shaderType == ShaderType::AMBIENT)
        {
            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());
        }
        else if (shaderType == ShaderType::DIFFUSE)
        {
            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());

            // flashlight(as Spotlight) on Camera 
            if (camera->isFlashLightOn()) {

                Shader::sendUniform(shaderProgram, "flashLight.position", camera->getPosition());
                Shader::sendUniform(shaderProgram, "flashLight.direction", camera->target);
            }
        }
        else if (shaderType == ShaderType::SPECULAR)
        {
            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());
            Shader::sendUniform(shaderProgram, "viewPos", camera->getPosition());
        }
        else if (shaderType == ShaderType::PHONG)
        {
            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());
            Shader::sendUniform(shaderProgram, "viewPos", camera->getPosition());

            // flashlight(as Spotlight) on Camera 
            if (camera->isFlashLightOn()) {

                Shader::sendUniform(shaderProgram, "flashLight.position", camera->getPosition());
                Shader::sendUniform(shaderProgram, "flashLight.direction", camera->target);
            }
        }
        else if (shaderType == ShaderType::BLINN)
        {
        }
        //std::cout << "Camera has changed" << std::endl;
    }
    else if (cameraChange == camChange::PROJECTION) {
        // zoom / fov changed
        Shader::sendUniform(shaderProgram, "projectionMatrix", camera->getProjectionMatrix());
        //std::cout << "Projection has changed" << std::endl;
    }
    else if (cameraChange == camChange::FLASHLIGHT) {          // camChange::FLASHLIGHT
        if (shaderType == ShaderType::DIFFUSE || shaderType == ShaderType::DIFFUSE_MODEL) {
            Shader::sendUniform(shaderProgram, "flashLight.isActive", camera->isFlashLightOn() ? 1 : 0);
            if (camera->isFlashLightOn()) {
                Shader::sendUniform(shaderProgram, "flashLight.position", camera->getPosition());
                Shader::sendUniform(shaderProgram, "flashLight.direction", camera->target);
            }
        }    
    }
    else if (cameraChange == camChange::SHOW_TEXTURE_DETAIL) {
        if (this->shaderType == ShaderType::DIFFUSE_MODEL || this->shaderType == ShaderType::SPECULAR_MODEL) {
            Shader::sendUniform(shaderProgram, "showTextureDetail", camera->getTextureDetail());
        }
    }
}
