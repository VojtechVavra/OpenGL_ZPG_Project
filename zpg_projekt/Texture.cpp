#include "Texture.hpp"
#include <iostream>

Texture::Texture(std::string texturePath, GLuint textureId) : texturePath(texturePath), textureId(textureId)
{
    m_name = getLastNameAfterDelimiter(texturePath);
}

Texture::~Texture()
{
    std::cout << "Deleting texture: " << m_name << "\n";
    this->Delete();
}

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

std::string Texture::getName()
{
    return texturePath;
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

std::string Texture::getLastNameAfterDelimiter(const std::string& path)
{
    // Hledání posledního vıskytu zpìtného lomítka
    size_t pos_backslash = path.find_last_of('\\');
    // Hledání posledního vıskytu dopøedného lomítka
    size_t pos_forwardslash = path.find_last_of('/');

    // Pokud ádné lomítko nebylo nalezeno, vrátíme celı øetìzec
    if (pos_backslash == std::string::npos && pos_forwardslash == std::string::npos) {
        return path;
    }

    // Vyberte vìtší z obou pozic, pokud nìkterá není npos
    size_t pos;
    if (pos_backslash == std::string::npos) {
        pos = pos_forwardslash;
    }
    else if (pos_forwardslash == std::string::npos) {
        pos = pos_backslash;
    }
    else {
        pos = std::max(pos_backslash, pos_forwardslash);
    }

    // Vrate podøetìzec za posledním lomítkem
    return path.substr(pos + 1);
}
