#include "Texture.hpp"

Texture::Texture(std::string texturePath, GLuint textureId) : texturePath(texturePath), textureId(textureId){ }


void Texture::Load() const {
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);  // GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glBindTexture(GL_TEXTURE_2D, textureId);
}


void Texture::LoadCubemap() const {
    glActiveTexture(GL_TEXTURE0);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);  // GL_REPEAT
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}

GLuint Texture::getTextureId()
{
    return textureId;
}
