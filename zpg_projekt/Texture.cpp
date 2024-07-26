#include "Texture.hpp"

Texture::Texture(std::string texturePath, GLuint textureId) : texturePath(texturePath), textureId(textureId){ }

// TODO: smazat, misto toho pouzit Bind() funkci
void Texture::Load() const {
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);  // GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::Bind() const {
    glActiveTexture(GL_TEXTURE0); // Aktivuje texture unit 0
    glBindTexture(GL_TEXTURE_2D, textureId); // Bind textury
}

void Texture::LoadCubemap() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}

GLuint Texture::getTextureId()
{
    return textureId;
}

void Texture::Delete()
{
    /**
     When you're done using the texture, delete it. This will set texname to 0 and
     delete all of the graphics card memory associated with the texture. If you
     don't call this method, the texture will stay in graphics card memory until you
     close the application.
    */
    glDeleteTextures(1, &textureId);
}
